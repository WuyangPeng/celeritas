#pragma once

#include "database_session_fixture.h"
#include "common/common_constant.h"
#include "common/core/celeritas_error.h"
#include "common/core/noexcept_safe_call_and_log.h"
#include "config/aggregate/detail/database_config_reader.h"

#include <boost/filesystem.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/test/unit_test.hpp>

template <typename SessionType>
void celeritas::database_session_fixture<SessionType>::run(awaitable_function func)
{
    if (!session_)
    {
        BOOST_FAIL("Session is not initialized, cannot run test.");
        return;
    }

    boost::asio::co_spawn(io_context_,
                          noexcept_safe_call_and_log_awaitable(std::move(func),
                                                               database_channel,
                                                               "database session test run error: "),
                          boost::asio::detached);

    io_context_.run();
    io_context_.restart();

    BOOST_CHECK(test_end_);
}

template <typename SessionType>
celeritas::database_session_fixture<SessionType>::session_shared_ptr celeritas::database_session_fixture<SessionType>::get_session() const
{
    return session_;
}

template <typename SessionType>
celeritas::database_session_fixture<SessionType>::const_database_config_shared_ptr celeritas::database_session_fixture<SessionType>::get_config() const
{
    return config_;
}

template <typename SessionType>
void celeritas::database_session_fixture<SessionType>::set_test_end(const bool test_end)
{
    test_end_ = test_end;
}

template <typename SessionType>
bool celeritas::database_session_fixture<SessionType>::is_test_end() const
{
    return test_end_;
}

template <typename SessionType>
void celeritas::database_session_fixture<SessionType>::do_pre_init()
{
}

template <typename SessionType>
void celeritas::database_session_fixture<SessionType>::init()
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

template <typename SessionType>
void celeritas::database_session_fixture<SessionType>::do_init_config(const database_type database_type)
{
    const auto current_path = boost::filesystem::current_path() / "config/tests/databases.xml";

    for (const auto configs = database_config_reader::load_config(current_path.string());
         const auto& config : *configs)
    {
        if (config->get_database_type() == database_type)
        {
            config_ = config;
            break;
        }
    }

    if (!config_)
    {
        throw celeritas_error{ "Database config not found for type in {}", current_path.string() };
    }
}

template <typename SessionType>
celeritas::database_session_fixture<SessionType>::io_context_type& celeritas::database_session_fixture<SessionType>::get_io_context()
{
    return io_context_;
}

template <typename SessionType>
void celeritas::database_session_fixture<SessionType>::set_session(session_shared_ptr session)
{
    session_ = std::move(session);
}

template <typename SessionType>
void celeritas::database_session_fixture<SessionType>::do_init()
{
    do_pre_init();
    init_config();
    init_session();
}

