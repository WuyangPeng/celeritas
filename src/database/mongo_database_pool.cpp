#include "common/logger.h"
#include "mongo_database_pool.h"
#include "common/common_fwd.h"

#include <boost/asio/experimental/promise.hpp>
#include <utility>

celeritas::mongo_database_pool::mongo_database_pool(boost::asio::io_context& io_context,
                                                    const std::string_view& uri,
                                                    const std::string_view& db_name,
                                                    size_t pool_size)
    : io_context_{ io_context },
      uri_{ uri },
      db_name_{ db_name },
      pool_size_{ pool_size }
{
}

celeritas::mongo_database_pool::awaitable_type celeritas::mongo_database_pool::async_initialize()
{
    for (auto i = 0u; i < pool_size_; ++i)
    {
        try
        {
            auto session = std::make_shared<mongo_database_session>(uri_, db_name_, io_context_);
            co_await session->async_connect();
            sessions_.emplace_back(session);
        }
        catch (const std::exception& error)
        {
            LOG_CHANNEL(database_channel, error) << "connect uri:" << uri_ << ",db_name:" << db_name_ << " error:" << error.what();
        }
        catch (...)
        {
            LOG_CHANNEL(database_channel, fatal) << "connect uri:" << uri_ << ",db_name:" << db_name_ << " unknown exception";
        }
    }
}

celeritas::mongo_database_pool::session_awaitable_type celeritas::mongo_database_pool::async_get_session()
{
    std::lock_guard lock{ mutex_ };

    if (!sessions_.empty())
    {
        auto session = sessions_.front();
        sessions_.pop_front();

        co_return session;
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

void celeritas::mongo_database_pool::release_session(const session_shared_ptr& session)
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
