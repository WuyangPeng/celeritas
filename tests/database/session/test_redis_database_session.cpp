#include "database/session/redis_database_session.h"
#include <boost/test/unit_test.hpp>
#include <boost/asio.hpp>

BOOST_AUTO_TEST_SUITE(redis_database_session_suite)

    BOOST_AUTO_TEST_CASE(test_redis_database_session_constructor)
    {
        boost::asio::io_context io_context;
        const std::string host = "localhost";
        const int port = 6379;
        const std::string user = "";
        const std::string password = "";
        const std::string uri = "redis://localhost:6379";
        const std::string db_name = "0";
        const int expire_seconds = 60;

        BOOST_CHECK_NO_THROW(
            celeritas::redis_database_session(host, port, user, password, uri, db_name, expire_seconds, io_context.get_executor())
        );
    }

    BOOST_AUTO_TEST_CASE(test_get_prefixed_key)
    {
        boost::asio::io_context io_context;
        celeritas::redis_database_session session("localhost", 6379, "", "", "redis://localhost:6379", "0", 60, io_context.get_executor());
        
        std::string key = "test_key";
        std::string prefixed = session.get_prefixed_key(key);
        BOOST_CHECK(!prefixed.empty());
    }

    BOOST_AUTO_TEST_CASE(test_get_expire_seconds_command)
    {
        boost::asio::io_context io_context;
        celeritas::redis_database_session session("localhost", 6379, "", "", "redis://localhost:6379", "0", 60, io_context.get_executor());

        auto command = session.get_expire_seconds_command(120);
        BOOST_CHECK(!command.empty());
    }

    BOOST_AUTO_TEST_CASE(test_get_commands_accessors)
    {
        boost::asio::io_context io_context;
        celeritas::redis_database_session session("localhost", 6379, "", "", "redis://localhost:6379", "0", 60, io_context.get_executor());

        BOOST_CHECK_NO_THROW(static_cast<void>(session.get_redis_key_commands()));
        BOOST_CHECK_NO_THROW(static_cast<void>(session.get_redis_string_commands()));
        BOOST_CHECK_NO_THROW(static_cast<void>(session.get_redis_hash_commands()));
        BOOST_CHECK_NO_THROW(static_cast<void>(session.get_redis_list_commands()));
        BOOST_CHECK_NO_THROW(static_cast<void>(session.get_redis_set_commands()));
        BOOST_CHECK_NO_THROW(static_cast<void>(session.get_redis_sorted_set_commands()));
    }

BOOST_AUTO_TEST_SUITE_END()
