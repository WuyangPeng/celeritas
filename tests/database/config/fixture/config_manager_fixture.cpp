#include "config_manager_fixture.h"
#include "common/core/celeritas_error.h"
#include "common/core/noexcept_safe_call_and_log.h"
#include "database/config/config_manager.h"
#include "database/config/config_table.tpp"
#include "database/config/mock/mock_config_database_pool.h"
#include "database/pool/database_pool_manager.h"

#include <boost/asio/steady_timer.hpp>

celeritas::config_manager_fixture::config_manager_fixture()
    : io_context_{}, test_end_{ false }
{
    const auto mock_pool = std::make_shared<mock_config_database_pool>();
    database_pool_manager::get_instance().set_mock_pool(mock_pool);
}

celeritas::config_manager_fixture::~config_manager_fixture()
{
    database_pool_manager::get_instance().set_mock_pool(nullptr);
    config_manager::get_instance().clear();
}

void celeritas::config_manager_fixture::spawn(awaitable_function func)
{
    boost::asio::co_spawn(io_context_,
                          noexcept_safe_call_and_log_awaitable(std::move(func),
                                                               database_channel,
                                                               "config manager fixture run error: "),
                          boost::asio::detached);
}

void celeritas::config_manager_fixture::run(awaitable_function func)
{
    spawn(std::move(func));

    io_context_.run();
    io_context_.restart();
    BOOST_CHECK(test_end_);
}

celeritas::config_manager_fixture::io_context_type& celeritas::config_manager_fixture::get_io_context()
{
    return io_context_;
}

void celeritas::config_manager_fixture::set_test_end(const bool test_end)
{
    this->test_end_ = test_end;
}

void celeritas::config_manager_fixture::check_time_refresh_valid()
{
    const auto optional_time_refresh = config_manager::get_instance().get_time_refresh(mock_config_database_pool::time_refresh_id);

    BOOST_REQUIRE(optional_time_refresh.has_value());
    BOOST_CHECK_EQUAL((*optional_time_refresh)->get_time_refresh_type(), 2);
    BOOST_CHECK_EQUAL((*optional_time_refresh)->get_parameter(), 3);
}

void celeritas::config_manager_fixture::spawn_writer(const atomic_int_shared_ptr& tasks_remaining, const atomic_bool_shared_ptr& stop_flag)
{
    spawn([this, tasks_remaining, stop_flag]() -> boost::asio::awaitable<void> {
        for (auto i = 0; i < 10; ++i)
        {
            config_manager::get_instance().reload_from_db(io_context_.get_executor(), "", 0);
            co_await boost::asio::steady_timer(io_context_.get_executor(), std::chrono::milliseconds{ 10 }).async_wait(boost::asio::use_awaitable);
        }
        *stop_flag = true;
        --(*tasks_remaining);
    });
}

void celeritas::config_manager_fixture::spawn_reader(const atomic_int_shared_ptr& tasks_remaining, const atomic_bool_shared_ptr& stop_flag)
{
    spawn([this, tasks_remaining, stop_flag]() -> boost::asio::awaitable<void> {
        while (!*stop_flag)
        {
            if (const auto result = config_manager::get_instance().get_time_refresh(mock_config_database_pool::time_refresh_id))
            {
                BOOST_CHECK_EQUAL((*result)->get_time_refresh_type(), 2);
            }
            co_await boost::asio::steady_timer(io_context_.get_executor(), std::chrono::milliseconds{ 5 }).async_wait(boost::asio::use_awaitable);
        }
        --(*tasks_remaining);
    });
}

celeritas::config_manager_fixture::void_awaitable celeritas::config_manager_fixture::check_load_one(const database_pool_shared_ptr& pool)
{
    auto exception_thrown = false;
    try
    {
        config_table<time_refresh> table{ time_refresh_db_name };

        co_await table.load_one(pool, 0);
    }
    catch (const celeritas_error&)
    {
        exception_thrown = true;
    }
    BOOST_CHECK(exception_thrown);
}
