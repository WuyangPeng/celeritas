#pragma once

#include "connection_pool.h"
#include "database_session_guard.tpp"
#include "common/logging/logger.h"
#include "database/basic/database_entity_change.h"

#include <boost/polymorphic_pointer_cast.hpp>

#include <functional>

template <typename SessionType>
celeritas::connection_pool<SessionType>::connection_pool(any_io_executor any_io_executor,
                                                         std::string host,
                                                         const int port,
                                                         std::string user,
                                                         std::string password,
                                                         std::string db_name,
                                                         const int min_connections,
                                                         const int max_connections)
    : any_io_executor_{ std::move(any_io_executor) },
      host_{ std::move(host) },
      port_{ port },
      user_{ std::move(user) },
      password_{ std::move(password) },
      uri_{},
      db_name_{ std::move(db_name) },
      expire_seconds_{},
      connections_{ 0 },
      min_connections_{ min_connections },
      max_connections_{ max_connections }
{
}

template <typename SessionType>
celeritas::connection_pool<SessionType>::connection_pool(any_io_executor any_io_executor,
                                                         std::string host,
                                                         const int port,
                                                         std::string user,
                                                         std::string password,
                                                         std::string db_name,
                                                         const int min_connections,
                                                         const int max_connections,
                                                         const int expire_seconds)
    : any_io_executor_{ std::move(any_io_executor) },
      host_{ std::move(host) },
      port_{ port },
      user_{ std::move(user) },
      password_{ std::move(password) },
      uri_{},
      db_name_{ std::move(db_name) },
      expire_seconds_{ expire_seconds },
      connections_{ 0 },
      min_connections_{ min_connections },
      max_connections_{ max_connections }
{
}

template <typename SessionType>
celeritas::connection_pool<SessionType>::connection_pool(any_io_executor any_io_executor,
                                                         std::string uri,
                                                         std::string db_name,
                                                         const int min_connections,
                                                         const int max_connections)
    : any_io_executor_{ std::move(any_io_executor) },
      host_{},
      port_{},
      user_{},
      password_{},
      uri_{ std::move(uri) },
      db_name_{ std::move(db_name) },
      expire_seconds_{},
      connections_{ 0 },
      min_connections_{ min_connections },
      max_connections_{ max_connections }
{
}

template <typename SessionType>
celeritas::connection_pool<SessionType>::void_awaitable_type celeritas::connection_pool<SessionType>::async_initialize()
{
    for (auto i = 0u; i < min_connections_; ++i)
    {
        ++connections_;
        if (!co_await this->async_one_initialize())
        {
            --connections_;
            break;
        }
    }
}

template <typename SessionType>
celeritas::connection_pool<SessionType>::database_session_guard_awaitable_type celeritas::connection_pool<SessionType>::async_get_session()
{
    auto session = try_get_existing_session();
    if (session != nullptr)
    {
        co_return database_session_guard_type{ session, boost::polymorphic_pointer_cast<class_type>(this->shared_from_this()) };
    }

    if (connections_.fetch_add(1) < max_connections_)
    {
        session = co_await try_create_session();
        if (session != nullptr)
        {
            co_return database_session_guard_type{ session, boost::polymorphic_pointer_cast<class_type>(this->shared_from_this()) };
        }
    }

    --connections_;
    session = co_await async_initiate_session();

    co_return database_session_guard_type{ session, boost::polymorphic_pointer_cast<class_type>(this->shared_from_this()) };
}

template <typename SessionType>
void celeritas::connection_pool<SessionType>::release_session(const session_shared_ptr& session)
{
    auto waiter = get_waiter_or_store_session(session);

    if (waiter != nullptr)
    {
        waiter(session);
    }
}

template <typename SessionType>
void celeritas::connection_pool<SessionType>::cleanup_database_by_duration()
{
    if (connections_ <= min_connections_)
    {
        return;
    }

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
celeritas::connection_pool<SessionType>::bool_awaitable_type celeritas::connection_pool<SessionType>::is_health()
{
    auto session = co_await async_get_session();

    co_return co_await session.get_session()->is_health();
}

template <typename SessionType>
celeritas::connection_pool<SessionType>::bool_awaitable_type celeritas::connection_pool<SessionType>::execute_changes(const const_database_entity_change_shared_ptr& database, int expiration_time)
{
    co_return co_await noexcept_safe_call_and_log_awaitable([self = boost::polymorphic_pointer_downcast<class_type>(shared_from_this()),
                                                                database = database,
                                                                expiration_time = expiration_time]() -> boost::asio::awaitable<bool> {
                                                                co_return co_await self->do_execute_changes(database, expiration_time);
                                                            },
                                                            database_channel,
                                                            false,
                                                            "execute changes error: ");
}

template <typename SessionType>
celeritas::database_pool_base::optional_database_entity_change_awaitable_type celeritas::connection_pool<SessionType>::select_one(const const_database_entity_change_shared_ptr& database, const database_field_container& field_name_container)
{
    auto session = co_await async_get_session();

    co_return co_await session.get_session()->select_one(database, field_name_container);
}

template <typename SessionType>
celeritas::database_pool_base::result_container_awaitable_type celeritas::connection_pool<SessionType>::select_all(const const_database_entity_change_shared_ptr& database, const database_field_container& field_name_container)
{
    auto session = co_await async_get_session();

    co_return co_await session.get_session()->select_all(database, field_name_container);
}

template <typename SessionType>
celeritas::database_pool_base::result_container_awaitable_type celeritas::connection_pool<SessionType>::select_page(const const_database_entity_change_shared_ptr& database,
                                                                                                                     const database_field_container& field_name_container,
                                                                                                                     const database_select_options& options)
{
    auto session = co_await async_get_session();

    co_return co_await session.get_session()->select_page(database, field_name_container, options);
}

template <typename SessionType>
celeritas::database_pool_base::int64_awaitable_type celeritas::connection_pool<SessionType>::select_count(const const_database_entity_change_shared_ptr& database)
{
    auto session = co_await async_get_session();

    co_return co_await session.get_session()->select_count(database);
}

template <typename SessionType>
celeritas::database_pool_base::int64_awaitable_type celeritas::connection_pool<SessionType>::select_count(const const_database_entity_change_shared_ptr& database,
                                                                                                          const database_select_options& options)
{
    auto session = co_await async_get_session();

    co_return co_await session.get_session()->select_count(database, options);
}

template <typename SessionType>
celeritas::connection_pool<SessionType>::bool_awaitable_type celeritas::connection_pool<SessionType>::async_one_initialize()
{
    try
    {
        co_await this->do_async_one_initialize();
        co_return true;
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(database_channel, error) << "connect host:" << host_ << ",port:" << port_ << " error:" << error.what();
        co_return false;
    }
    catch (...)
    {
        LOG_CHANNEL(database_channel, fatal) << "connect host:" << host_ << ",port:" << port_ << " unknown exception";
        co_return false;
    }
}

template <typename SessionType>
celeritas::connection_pool<SessionType>::void_awaitable_type celeritas::connection_pool<SessionType>::do_async_one_initialize()
{
    auto session = std::make_shared<SessionType>(host_, port_, user_, password_, uri_, db_name_, expire_seconds_, any_io_executor_);
    co_await session->async_connect();

    LOG_CHANNEL(database_channel, info) << "connect host:" << host_ << ",port:" << port_ << " success.";

    std::lock_guard lock{ mutex_ };

    sessions_.emplace_back(session);
}

template <typename SessionType>
celeritas::connection_pool<SessionType>::session_shared_ptr celeritas::connection_pool<SessionType>::try_get_existing_session()
{
    std::lock_guard lock{ mutex_ };

    if (!sessions_.empty())
    {
        auto session = sessions_.back();
        session->set_last_heartbeat();
        sessions_.pop_back();

        return session;
    }

    return nullptr;
}

template <typename SessionType>
celeritas::connection_pool<SessionType>::session_awaitable_type celeritas::connection_pool<SessionType>::try_create_session()
{
    if (co_await this->async_one_initialize())
    {
        co_return try_get_existing_session();
    }

    LOG_CHANNEL(database_channel, error) << "Failed to create new database connection, waiting for existing connection.";

    co_return nullptr;
}

template <typename SessionType>
celeritas::connection_pool<SessionType>::session_awaitable_type celeritas::connection_pool<SessionType>::async_initiate_session()
{
    // 如果没有可用会话，将当前协程挂起并加入等待队列。
    // 使用 async_initiate 创建一个自定义的异步操作。
    co_return co_await boost::asio::async_initiate<decltype(boost::asio::use_awaitable), void(session_shared_ptr)>(
        [&](auto handler) {
            auto executor = handler.get_executor();
            std::lock_guard lock{ mutex_ };

            waiters_.emplace_back(
                [handler = std::move(handler), executor = std::move(executor)](session_shared_ptr session) mutable {
                    // 当会话被释放时，使用 dispatch 确保 handler 在其原始的执行器上运行，
                    // 这对于协程的正确恢复至关重要。
                    boost::asio::dispatch(executor,
                                          [handler = std::move(handler), session = std::move(session)]() mutable {
                                              handler(std::move(session));
                                          });
                });
        },
        boost::asio::use_awaitable);
}

template <typename SessionType>
celeritas::database_pool_base::bool_awaitable_type celeritas::connection_pool<SessionType>::do_execute_changes(const const_database_entity_change_shared_ptr& database, int expiration_time)
{
    if (!database->is_modify())
    {
        co_return true;
    }

    auto session = co_await async_get_session();

    co_await session.get_session()->execute_changes(database, expiration_time);

    co_return true;
}

template <typename SessionType>
celeritas::connection_pool<SessionType>::waiter_type celeritas::connection_pool<SessionType>::get_waiter_or_store_session(const session_shared_ptr& session)
{
    std::lock_guard lock{ mutex_ };

    if (!waiters_.empty())
    {
        // 如果有等待的协程，直接将会话给它
        auto waiter = std::move(waiters_.front());
        waiters_.pop_front();
        session->set_last_heartbeat();
        return waiter;
    }

    sessions_.emplace_back(session);
    return nullptr;
}