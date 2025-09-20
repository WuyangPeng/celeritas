#include "mysql_database_pool.h"

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <utility>

celeritas::mysql_database_pool::mysql_database_pool(boost::asio::io_context& io_context,
                                                    std::string host,
                                                    uint16_t port,
                                                    std::string user,
                                                    std::string password,
                                                    std::string db_name,
                                                    const size_t pool_size,
                                                    boost::asio::ssl::context* ssl_context)
    : io_context_{ io_context },
      ssl_context_{ ssl_context },
      host_{ std::move(host) },
      port_{ port },
      user_{ std::move(user) },
      password_{ std::move(password) },
      db_name_{ std::move(db_name) },
      pool_size_{ pool_size }
{
}

celeritas::mysql_database_pool::awaitable_type celeritas::mysql_database_pool::async_initialize()
{
    for (auto i = 0u; i < pool_size_; ++i)
    {
        auto session = std::make_shared<mysql_database_session>(io_context_, ssl_context_);
        co_await session->async_connect(host_, port_, user_, password_, db_name_);
        sessions_.emplace_back(session);
    }
}

celeritas::mysql_database_pool::session_awaitable_type celeritas::mysql_database_pool::async_get_session()
{
    std::unique_lock lock{ mutex_ };
    condition_variable_.wait(lock, [this] {
        return !sessions_.empty();
    });
    auto session = sessions_.front();
    sessions_.pop_front();
    co_return session;
}

void celeritas::mysql_database_pool::release_session(const session_shared_ptr& session)
{
    std::lock_guard lock{ mutex_ };
    sessions_.emplace_back(session);
    condition_variable_.notify_one();
}
