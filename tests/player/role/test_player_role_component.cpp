#include "database/database_pool_manager.h"
#include "player/mock/mock_database_pool.h"
#include "player/mock/mock_player_state.h"
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
              player_state_{ io_context_ },
              mock_pool_{ std::make_shared<celeritas::mock_database_pool>() },
              login_request_{}
        {
            login_request_.set_device_id("test_device");
            login_request_.set_app_version("1.0.0");

            celeritas::database_pool_manager::get_instance().set_mock_pool(mock_pool_);
        }

        void run_io_context()
        {
            io_context_.restart();
            io_context_.run();
        }

        boost::asio::io_context io_context_;
        celeritas::mock_player_state player_state_;
        std::shared_ptr<celeritas::mock_database_pool> mock_pool_;
        celeritas::proto::service::service_login_request login_request_;
    };

    BOOST_FIXTURE_TEST_CASE(test_change_name, test_fixture)
    {
        celeritas::player_role_component role_component{ &player_state_, login_request_ };

        boost::asio::co_spawn(io_context_, role_component.on_load_db(), boost::asio::detached);
        run_io_context();

        const std::string new_name{ "new_test_name" };
        boost::asio::co_spawn(io_context_, role_component.change_name("new_test_name", "new_test_name"), boost::asio::detached);
        run_io_context();

        BOOST_CHECK_EQUAL(role_component.get_name(), new_name);
    }

    BOOST_FIXTURE_TEST_CASE(test_is_modify, test_fixture)
    {
        celeritas::player_role_component role_component{ &player_state_, login_request_ };

        boost::asio::co_spawn(io_context_, role_component.on_load_db(), boost::asio::detached);
        run_io_context();

        boost::asio::co_spawn(io_context_, role_component.save_db(), boost::asio::detached);
        run_io_context();

        BOOST_CHECK(!role_component.is_modify());

        boost::asio::co_spawn(io_context_, role_component.change_name("new_name", "new_name"), boost::asio::detached);
        run_io_context();

        BOOST_CHECK(role_component.is_modify());
    }

    BOOST_FIXTURE_TEST_CASE(test_save_db_resets_modify_flag, test_fixture)
    {
        celeritas::player_role_component role_component{ &player_state_, login_request_ };

        boost::asio::co_spawn(io_context_, role_component.on_load_db(), boost::asio::detached);
        run_io_context();

        boost::asio::co_spawn(io_context_, role_component.save_db(), boost::asio::detached);
        run_io_context();

        BOOST_CHECK(!role_component.is_modify());

        boost::asio::co_spawn(io_context_, role_component.change_name("another_name", "another_name"), boost::asio::detached);
        run_io_context();

        BOOST_CHECK(role_component.is_modify());

        boost::asio::co_spawn(io_context_, role_component.save_db(), boost::asio::detached);
        run_io_context();

        BOOST_CHECK(!role_component.is_modify());
    }

    BOOST_FIXTURE_TEST_CASE(test_load_db, test_fixture)
    {
        celeritas::player_role_component role_component{ &player_state_, login_request_ };
        boost::asio::co_spawn(io_context_, role_component.on_load_db(), boost::asio::detached);
        run_io_context();

        BOOST_CHECK_EQUAL(role_component.get_name(), "test_name");
    }

    BOOST_FIXTURE_TEST_CASE(test_set_login, test_fixture)
    {
        celeritas::player_role_component role_component{ &player_state_, login_request_ };

        boost::asio::co_spawn(io_context_, role_component.on_load_db(), boost::asio::detached);
        run_io_context();

        boost::asio::co_spawn(io_context_, role_component.save_db(), boost::asio::detached);
        run_io_context();

        BOOST_CHECK(!role_component.is_modify());

        celeritas::proto::service::service_login_request new_login_request;
        new_login_request.set_device_id("new_device_id");
        new_login_request.set_app_version("2.0.0");
        role_component.set_login(new_login_request);

        BOOST_CHECK(role_component.is_modify());
        BOOST_CHECK_EQUAL(role_component.get_device_id(), new_login_request.device_id());
        BOOST_CHECK_EQUAL(role_component.get_app_version(), new_login_request.app_version());
    }

BOOST_AUTO_TEST_SUITE_END()
