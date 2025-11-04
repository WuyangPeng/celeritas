#pragma once

#include "connection_pool_base.h"
#include "common/logger.h"

template <typename SessionType>
celeritas::connection_pool_base<SessionType>::connection_pool_base(io_context_type& io_context,
                                                                   std::string host,
                                                                   const int port,
                                                                   std::string user,
                                                                   std::string password,
                                                                   std::string db_name,
                                                                   const int min_connections,
                                                                   const int max_connections)
    : io_context_{ io_context },
      host_{ std::move(host) },
      port_{ port },
      user_{ std::move(user) },
      password_{ std::move(password) },
      uri_{},
      db_name_{ std::move(db_name) },
      connections_{ 0 },
      min_connections_{ min_connections },
      max_connections_{ max_connections }
{
}

template <typename SessionType>
celeritas::connection_pool_base<SessionType>::connection_pool_base(io_context_type& io_context,
                                                                   std::string host,
                                                                   const int port,
                                                                   std::string user,
                                                                   std::string password,
                                                                   const int min_connections,
                                                                   const int max_connections)
    : io_context_{ io_context },
      host_{ std::move(host) },
      port_{ port },
      user_{ std::move(user) },
      password_{ std::move(password) },
      uri_{},
      db_name_{},
      connections_{ 0 },
      min_connections_{ min_connections },
      max_connections_{ max_connections }
{
}

template <typename SessionType>
celeritas::connection_pool_base<SessionType>::connection_pool_base(io_context_type& io_context,
                                                                   std::string uri,
                                                                   std::string db_name,
                                                                   const int min_connections,
                                                                   const int max_connections)
    : io_context_{ io_context },
      host_{},
      port_{},
      user_{},
      password_{},
      uri_{ std::move(uri) },
      db_name_{ std::move(db_name) },
      connections_{ 0 },
      min_connections_{ min_connections },
      max_connections_{ max_connections }
{
}

template <typename SessionType>
celeritas::connection_pool_base<SessionType>::void_awaitable_type celeritas::connection_pool_base<SessionType>::async_initialize()
{
    for (auto i = 0u; i < min_connections_; ++i)
    {
        co_await this->async_one_initialize();
    }
}

template <typename SessionType>
celeritas::connection_pool_base<SessionType>::session_awaitable_type celeritas::connection_pool_base<SessionType>::async_get_session()
{
    auto session = try_get_existing_session();
    if (session != nullptr)
    {
        co_return session;
    }

    if (connections_ < max_connections_)
    {
        co_await this->async_one_initialize();

        session = try_get_existing_session();
        if (session != nullptr)
        {
            co_return session;
        }
    }

    co_return co_await async_initiate_session();
}

template <typename SessionType>
void celeritas::connection_pool_base<SessionType>::release_session(const session_shared_ptr& session)
{
    std::lock_guard lock{ mutex_ };

    if (!waiters_.empty())
    {
        // 如果有等待的协程，直接将会话给它
        auto waiter = std::move(waiters_.front());
        waiters_.pop_front();
        session->set_last_heartbeat();
        waiter(session);
    }
    else
    {
        sessions_.emplace_back(session);
    }
}

template <typename SessionType>
void celeritas::connection_pool_base<SessionType>::cleanup_database_by_duration()
{
    std::lock_guard lock{ mutex_ };

    for (auto iter = sessions_.begin(); iter != sessions_.end();)
    {
        if (connections_ <= min_connections_)
        {
            return;
        }

        if ((*iter)->is_expired())
        {
            iter = sessions_.erase(iter);
            --connections_;
        }
        else
        {
            ++iter;
        }
    }
}

template <typename SessionType>
bool celeritas::connection_pool_base<SessionType>::is_health()
{
    std::lock_guard lock{ mutex_ };

    return true;
}

template <typename SessionType>
celeritas::connection_pool_base<SessionType>::void_awaitable_type celeritas::connection_pool_base<SessionType>::async_one_initialize()
{
    try
    {
        co_await this->do_async_one_initialize();
        ++connections_;
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(database_channel, error) << "connect host:" << host_ << ",port:" << port_ << " error:" << error.what();
    }
    catch (...)
    {
        LOG_CHANNEL(database_channel, fatal) << "connect host:" << host_ << ",port:" << port_ << " unknown exception";
    }
}

template <typename SessionType>
celeritas::connection_pool_base<SessionType>::void_awaitable_type celeritas::connection_pool_base<SessionType>::do_async_one_initialize()
{
    auto session = std::make_shared<SessionType>(host_, port_, user_, password_, uri_, db_name_, io_context_);
    co_await session->async_connect();

    LOG_CHANNEL(database_channel, info) << "connect host:" << host_ << ",port:" << port_ << " success.";

    std::lock_guard lock{ mutex_ };

    sessions_.emplace_back(session);
}

template <typename SessionType>
celeritas::connection_pool_base<SessionType>::session_shared_ptr celeritas::connection_pool_base<SessionType>::try_get_existing_session()
{
    std::lock_guard lock{ mutex_ };

    if (!sessions_.empty())
    {
        auto session = sessions_.front();
        session->set_last_heartbeat();
        sessions_.pop_front();

        return session;
    }

    return nullptr;
}

template <typename SessionType>
celeritas::connection_pool_base<SessionType>::session_awaitable_type celeritas::connection_pool_base<SessionType>::async_initiate_session()
{
    // 如果没有可用会话，将当前协程挂起并加入等待队列。
    // 使用 async_initiate 创建一个自定义的异步操作。
    co_return boost::asio::async_initiate<decltype(boost::asio::use_awaitable), void(session_shared_ptr)>(
        [&](auto handler) {
            std::lock_guard lock{ mutex_ };

            waiters_.emplace_back(
                [handler = std::move(handler)](session_shared_ptr session) {
                    // 当会话被释放时，使用 dispatch 确保 handler 在其原始的执行器上运行，
                    // 这对于协程的正确恢复至关重要。
                    boost::asio::dispatch(handler.get_executor(),
                                          [handler = std::move(handler), session = std::move(session)] {
                                              handler(session);
                                          });
                });
        },
        boost::asio::use_awaitable);
}