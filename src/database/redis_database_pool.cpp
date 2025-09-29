#include "common/logger.h"
#include "redis_database_pool.h"
#include "common/common_fwd.h"

#include "common/logger.h"
#include "common/common_fwd.h"

#include <boost/asio/experimental/promise.hpp>
#include <utility>

celeritas::redis_database_pool::redis_database_pool(boost::asio::io_context& io_context,
                                                    const std::string_view& host,
                                                    uint16_t port,
                                                    const std::string_view& password,
                                                    int min_connections,
                                                    int max_connections)
    : io_context_{ io_context },
      host_{ host },
      port_{ port },
      password_{ password },
      connections_{ 0 },
      min_connections_{ min_connections },
      max_connections_{ max_connections }
{
}

celeritas::redis_database_pool::awaitable_type celeritas::redis_database_pool::async_initialize()
{
    for (auto i = 0u; i < min_connections_; ++i)
    {
        co_await async_one_initialize();
        ++connections_;
    }
}

celeritas::redis_database_pool::session_awaitable_type celeritas::redis_database_pool::async_get_session()
{
    std::lock_guard lock{ mutex_ };

    if (!sessions_.empty())
    {
        auto session = sessions_.front();
        sessions_.pop_front();

        co_return session;
    }

    if (connections_ < max_connections_)
    {
        co_await async_one_initialize();

        if (!sessions_.empty())
        {
            auto session = sessions_.front();
            sessions_.pop_front();

            co_return session;
        }
    }

    // 如果没有可用会话，将当前协程挂起并加入等待队列。
    // 使用 async_initiate 创建一个自定义的异步操作。
    co_return co_await boost::asio::async_initiate<decltype(boost::asio::use_awaitable), void(session_shared_ptr)>(
        [&](auto handler) {
            std::lock_guard lock_guard{ mutex_ };
            waiters_.emplace_back(
                [handler = std::move(handler)](session_shared_ptr session) mutable {
                    // 当会话被释放时，使用 dispatch 确保 handler 在其原始的执行器上运行，
                    // 这对于协程的正确恢复至关重要。
                    boost::asio::dispatch(handler.get_executor(),
                                          [handler = std::move(handler), session = std::move(session)]() mutable {
                                              handler(session);
                                          });
                });
        },
        boost::asio::use_awaitable);
}

void celeritas::redis_database_pool::release_session(const session_shared_ptr& session)
{
    std::lock_guard lock{ mutex_ };

    if (!waiters_.empty())
    {
        // 如果有等待的协程，直接将会话给它
        auto waiter = std::move(waiters_.front());
        waiters_.pop_front();
        waiter(session);
    }
    else
    {
        sessions_.emplace_back(session);
    }
}

celeritas::redis_database_pool::awaitable_type celeritas::redis_database_pool::async_one_initialize()
{
    try
    {
        auto session = std::make_shared<redis_database_session>(host_, port_, password_, io_context_);
        co_await session->async_connect();
        sessions_.emplace_back(session);

        LOG_CHANNEL(database_channel, info) << "connect host:" << host_ << ",port:" << port_ << " success.";
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
