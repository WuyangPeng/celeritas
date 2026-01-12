#include "database/basic/database_entity_change.h"
#include "database/session/mongo_database_session.h"

#include <boost/asio.hpp>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(mongo_database_session_suite)

    BOOST_AUTO_TEST_CASE(test_mongo_database_session_constructor)
    {
        boost::asio::io_context io_context;
        const std::string host = "localhost";
        const int port = 27017;
        const std::string user = "user";
        const std::string password = "password";
        const std::string uri = "mongodb://user:password@localhost:27017";
        const std::string db_name = "test_db";
        const int expire_seconds = 60;

        BOOST_CHECK_NO_THROW(
            celeritas::mongo_database_session(host, port, user, password, uri, db_name, expire_seconds, io_context.get_executor())
            );
    }

BOOST_AUTO_TEST_SUITE_END()
