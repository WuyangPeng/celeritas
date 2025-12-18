#include "player/mock_database_pool.h"
#include "player/mock_player_state.h"
#include "player/role/player_role_component.h"
#include "proto/service/player.pb.h"

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(player_role_component_suite)

    struct test_fixture
    {
        test_fixture()
            : io_context_{},
              player_state{ io_context_ },
              mock_pool_{ std::make_shared<celeritas::mock_database_pool>() },
              login_request{}
        {
            login_request.set_device_id("test_device");
            login_request.set_app_version("1.0.0");
        }

        void run_io_context()
        {
            io_context_.restart();
            io_context_.run();
        }

        boost::asio::io_context io_context_;
        celeritas::mock_player_state player_state;
        std::shared_ptr<celeritas::mock_database_pool> mock_pool_;
        celeritas::proto::service::service_login_request login_request;
    };

    BOOST_FIXTURE_TEST_CASE(test_change_name, test_fixture)
    {
        celeritas::player_role_component role_component{ &player_state, login_request };
        role_component.set_mock_database_pool(mock_pool_);

        boost::asio::co_spawn(io_context_, role_component.on_load_db(), boost::asio::detached);
        run_io_context();

        const std::string new_name{ "new_test_name" };
        role_component.change_name(new_name);

        BOOST_CHECK_EQUAL(role_component.get_name(), new_name);
    }

BOOST_AUTO_TEST_SUITE_END()
