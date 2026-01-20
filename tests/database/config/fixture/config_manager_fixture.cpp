#include "config_manager_fixture.h"
#include "common/core/noexcept_safe_call_and_log.h"
#include "database/config/config_manager.h"
#include "database/config/mock/mock_config_database_pool.h"
#include "database/pool/database_pool_manager.h"

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

void celeritas::config_manager_fixture::run(awaitable_function func)
{
    boost::asio::co_spawn(io_context_,
                          noexcept_safe_call_and_log_awaitable(std::move(func),
                                                               database_channel,
                                                               "config manager fixture run error: "),
                          boost::asio::detached);

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