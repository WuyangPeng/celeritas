#include "config_manager_fixture.h"
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
}

void celeritas::config_manager_fixture::run(awaitable_function func)
{
    boost::asio::co_spawn(io_context_, std::move(func), boost::asio::detached);
    io_context_.run();
    io_context_.restart();
    BOOST_CHECK(test_end_);
}

celeritas::config_manager_fixture::io_context_type& celeritas::config_manager_fixture::get_io_context()
{
    return io_context_;
}

void celeritas::config_manager_fixture::set_test_end(bool test_end)
{
    this->test_end_ = test_end;
}