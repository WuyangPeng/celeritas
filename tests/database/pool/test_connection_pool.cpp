#include "database/pool/connection_pool.tpp"
#include "database/basic/database_entity_change.h"
#include "database/database_fwd.h"
#include "database/pool/database_session_guard.tpp"

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/test/unit_test.hpp>
#include <vector>
#include <string>

namespace celeritas
{
    // A mock session class for testing connection_pool
    class mock_database_session final
    {
    public:
        // Constructor signature matches what connection_pool.tpp uses
        mock_database_session(const std::string&, int, const std::string&, const std::string&, const std::string&, const std::string&, int, boost::asio::any_io_executor)
        {
        }

        boost::asio::awaitable<void> async_connect()
        {
            co_return;
        }

        void set_last_heartbeat()
        {
        }

        bool is_expired() const
        {
            return false;
        }

        boost::asio::awaitable<bool> is_health()
        {
            co_return true;
        }

        boost::asio::awaitable<void> execute_changes(const std::shared_ptr<const database_entity_change>&, int)
        {
            co_return;
        }

        boost::asio::awaitable<std::optional<database_entity_change> > select_one(const std::shared_ptr<const database_entity_change>&, const std::vector<std::string>&)
        {
            co_return std::nullopt;
        }

        boost::asio::awaitable<std::vector<database_entity_change> > select_all(const std::shared_ptr<const database_entity_change>&, const std::vector<std::string>&)
        {
            co_return std::vector<database_entity_change>{};
        }
    };
} // namespace celeritas

namespace
{
    // Fixture for connection_pool tests, following the project's style
    struct connection_pool_fixture
    {
        connection_pool_fixture()
            : io_context_{},
              pool_{ std::make_shared<celeritas::connection_pool<celeritas::mock_database_session> >(
                  io_context_.get_executor(),
                  "127.0.0.1",
                  3306,
                  "user",
                  "password",
                  "test_db",
                  1, // min_connections
                  5 // max_connections
                  ) }
        {
        }

        void run_io_context()
        {
            io_context_.restart();
            io_context_.run();
        }

        boost::asio::io_context io_context_;
        std::shared_ptr<celeritas::connection_pool<celeritas::mock_database_session> > pool_;
    };
}

BOOST_FIXTURE_TEST_SUITE(connection_pool_suite, connection_pool_fixture)

    BOOST_AUTO_TEST_CASE(test_get_and_release_session)
    {
        boost::asio::co_spawn(io_context_, [&]() -> boost::asio::awaitable<void> {
            co_await pool_->async_initialize();
            auto session_guard = co_await pool_->async_get_session();
            BOOST_CHECK(session_guard.get_session() != nullptr);
        }, boost::asio::detached);

        run_io_context();
    }

BOOST_AUTO_TEST_SUITE_END()
