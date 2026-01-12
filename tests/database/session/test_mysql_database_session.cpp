#include "database/session/mysql_database_session.h"
#include "database/basic/database_entity_change.h"
#include <boost/test/unit_test.hpp>
#include <boost/asio.hpp>

BOOST_AUTO_TEST_SUITE(mysql_database_session_suite)

    BOOST_AUTO_TEST_CASE(test_mysql_database_session_constructor)
    {
        boost::asio::io_context io_context;
        const std::string host = "localhost";
        const int port = 3306;
        const std::string user = "user";
        const std::string password = "password";
        const std::string uri = ""; // URI might not be used for MySQL in this constructor signature directly if host/port are separate
        const std::string db_name = "test_db";
        const int expire_seconds = 60;

        // Assuming no SSL context is fine for basic construction test
        BOOST_CHECK_NO_THROW(
            celeritas::mysql_database_session(host, port, user, password, uri, db_name, expire_seconds, io_context.get_executor())
        );
    }

BOOST_AUTO_TEST_SUITE_END()
