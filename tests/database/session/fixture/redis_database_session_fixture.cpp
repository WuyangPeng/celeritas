#include "redis_database_session_fixture.h"
#include "common/core/celeritas_error.h"
#include "config/aggregate/detail/database_config_reader.h"
#include "config/basic/database_type.h"
#include "database/pool/database_pool_manager.h"

#include <boost/filesystem.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/test/unit_test.hpp>

celeritas::redis_database_session_fixture::redis_database_session_fixture()
    : io_context_{},
      session_{},
      config_{},
      test_end_{ false }
{
    init();
}

void celeritas::redis_database_session_fixture::run(awaitable_function func)
{
    if (!session_)
    {
        BOOST_FAIL("Session is not initialized, cannot run test.");
        return;
    }

    boost::asio::co_spawn(io_context_, std::move(func), boost::asio::detached);
    io_context_.run();
    io_context_.restart();
    BOOST_CHECK(test_end_);
}

celeritas::redis_database_session_fixture::redis_database_session_shared_ptr celeritas::redis_database_session_fixture::get_session() const
{
    return session_;
}

celeritas::redis_database_session_fixture::const_database_config_shared_ptr celeritas::redis_database_session_fixture::get_config() const
{
    return config_;
}

void celeritas::redis_database_session_fixture::set_test_end(const bool test_end)
{
    test_end_ = test_end;
}

bool celeritas::redis_database_session_fixture::is_test_end() const
{
    return test_end_;
}

void celeritas::redis_database_session_fixture::init()
{
    try
    {
        do_init();
    }
    catch (const std::exception& error)
    {
        BOOST_FAIL("Failed to load database config: " << error.what());
    }
}

void celeritas::redis_database_session_fixture::do_init()
{
    init_config();

    init_session();
}

void celeritas::redis_database_session_fixture::init_config()
{
    const auto current_path = boost::filesystem::current_path() / "config/tests/databases.xml";

    for (const auto configs = database_config_reader::load_config(current_path.string());
         const auto& cfg : *configs)
    {
        if (cfg->get_database_type() == database_type::redis)
        {
            config_ = cfg;
            break;
        }
    }

    if (!config_)
    {
        throw celeritas_error{ "Redis config not found in {}", current_path.string() };
    }
}

void celeritas::redis_database_session_fixture::init_session()
{
    session_ = std::make_shared<redis_database_session>(config_->get_host(),
                                                        config_->get_port(),
                                                        config_->get_user(),
                                                        config_->get_password(),
                                                        "",
                                                        config_->get_db_name(),
                                                        config_->get_expire_seconds(),
                                                        io_context_.get_executor());
}
