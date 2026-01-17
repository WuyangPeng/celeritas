#include "mongo_database_session_fixture.h"
#include "common/common_constant.h"
#include "common/core/celeritas_error.h"
#include "common/core/noexcept_safe_call_and_log.h"
#include "config/aggregate/detail/database_config_reader.h"
#include "config/basic/database_type.h"
#include "database/pool/database_pool_manager.h"

#include <boost/filesystem.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/test/unit_test.hpp>

celeritas::mongo_database_session_fixture::mongo_database_session_fixture()
    : io_context_{},
      session_{},
      config_{},
      test_end_{ false }
{
    init();
}

void celeritas::mongo_database_session_fixture::run(awaitable_function func)
{
    if (!session_)
    {
        BOOST_FAIL("Session is not initialized, cannot run test.");
        return;
    }

    boost::asio::co_spawn(io_context_,
                          noexcept_safe_call_and_log_awaitable(std::move(func),
                                                               database_channel,
                                                               "mongo database session test run error: "),
                          boost::asio::detached);

    io_context_.run();
    io_context_.restart();
    BOOST_CHECK(test_end_);
}

celeritas::mongo_database_session_fixture::mongo_database_session_shared_ptr celeritas::mongo_database_session_fixture::get_session() const
{
    return session_;
}

celeritas::mongo_database_session_fixture::const_database_config_shared_ptr celeritas::mongo_database_session_fixture::get_config() const
{
    return config_;
}

void celeritas::mongo_database_session_fixture::set_test_end(const bool test_end)
{
    test_end_ = test_end;
}

bool celeritas::mongo_database_session_fixture::is_test_end() const
{
    return test_end_;
}

void celeritas::mongo_database_session_fixture::init()
{
    database_pool_manager::create_mongo_instance();

    try
    {
        do_init();
    }
    catch (const std::exception& error)
    {
        BOOST_FAIL("Failed to load database config: " << error.what());
    }
}

void celeritas::mongo_database_session_fixture::do_init()
{
    init_config();

    init_session();
}

void celeritas::mongo_database_session_fixture::init_config()
{
    const auto current_path = boost::filesystem::current_path() / "config/tests/databases.xml";

    for (const auto configs = database_config_reader::load_config(current_path.string());
         const auto& config : *configs)
    {
        if (config->get_database_type() == database_type::mongo)
        {
            config_ = config;
            break;
        }
    }

    if (!config_)
    {
        throw celeritas_error{ "Mongo config not found in {}", current_path.string() };
    }
}

void celeritas::mongo_database_session_fixture::init_session()
{
    const auto uri = "mongodb://" + config_->get_user() + ":" + config_->get_password() + "@" + config_->get_host() + ":" + std::to_string(config_->get_port()) + "/" + config_->get_db_name();

    session_ = std::make_shared<mongo_database_session>(config_->get_host(),
                                                        config_->get_port(),
                                                        config_->get_user(),
                                                        config_->get_password(),
                                                        uri,
                                                        config_->get_db_name(),
                                                        config_->get_expire_seconds(),
                                                        io_context_.get_executor());
}
