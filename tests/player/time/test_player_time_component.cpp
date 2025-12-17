#include "common/time_helper.h"
#include "database/document/player_time_refresh.h"
#include "player/mock_database_pool.h"
#include "player/mock_player_component.h"
#include "player/mock_player_state.h"
#include "player/time/player_time_component.h"
#include "player/time/time_refresh_type.h"

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/test/unit_test.hpp>

namespace
{
    struct player_time_component_fixture
    {
        player_time_component_fixture()
            : io_context_{},
              mock_player_state_{ io_context_ },
              mock_pool_{ std::make_shared<celeritas::mock_database_pool>() },
              component_{ &mock_player_state_ }
        {
            component_.set_mock_database_pool(mock_pool_);

            mock_player_state_.set_mock_player_component(std::make_shared<celeritas::mock_player_component>(&mock_player_state_));
        }

        void run_io_context()
        {
            io_context_.restart();
            io_context_.run();
        }

        boost::asio::io_context io_context_;
        celeritas::mock_player_state mock_player_state_;
        std::shared_ptr<celeritas::mock_database_pool> mock_pool_;
        celeritas::player_time_component component_;
    };
}

BOOST_FIXTURE_TEST_SUITE(player_time_component_suite, player_time_component_fixture)

    BOOST_AUTO_TEST_CASE(test_initial_state)
    {
        boost::asio::co_spawn(io_context_, component_.on_load_db(), boost::asio::detached);
        run_io_context();

        BOOST_CHECK(!component_.is_modify());
    }

    BOOST_AUTO_TEST_CASE(test_register_and_remove_timer)
    {
        boost::asio::co_spawn(io_context_, component_.on_load_db(), boost::asio::detached);
        run_io_context();

        const celeritas::player_time_refresh_key key{ celeritas::time_refresh_type::daily, 1, 1 };
        constexpr auto component_type = celeritas::player_component_type::user;

        BOOST_CHECK(!component_.is_modify());

        component_.register_timer(component_type, key);
        BOOST_CHECK(component_.is_modify());

        boost::asio::co_spawn(io_context_, component_.save_db(), boost::asio::detached);
        run_io_context();

        BOOST_CHECK(!component_.is_modify());

        component_.remove_timer(component_type, key);
        BOOST_CHECK(component_.is_modify());
    }

    BOOST_AUTO_TEST_CASE(test_save_db_with_modifications)
    {
        boost::asio::co_spawn(io_context_, component_.on_load_db(), boost::asio::detached);
        run_io_context();

        const celeritas::player_time_refresh_key key{ celeritas::time_refresh_type::daily, 1, 1 };
        component_.register_timer(celeritas::player_component_type::user, key);
        BOOST_CHECK(component_.is_modify());

        boost::asio::co_spawn(io_context_, component_.save_db(), boost::asio::detached);
        run_io_context();

        BOOST_CHECK_EQUAL(mock_pool_->get_execute_changes_call_count(), 1);
        BOOST_CHECK(!component_.is_modify());
    }

    BOOST_AUTO_TEST_CASE(test_save_db_without_modifications)
    {
        boost::asio::co_spawn(io_context_, component_.on_load_db(), boost::asio::detached);
        run_io_context();

        BOOST_CHECK(!component_.is_modify());

        boost::asio::co_spawn(io_context_, component_.save_db(), boost::asio::detached);
        run_io_context();

        BOOST_CHECK_EQUAL(mock_pool_->get_execute_changes_call_count(), 0);
    }

    BOOST_AUTO_TEST_CASE(test_on_load_db)
    {
        const celeritas::player_time_refresh_key key{ celeritas::time_refresh_type::weekly, 123, 456 };

        boost::asio::co_spawn(io_context_, component_.on_load_db(), boost::asio::detached);
        run_io_context();

        const auto player_time_refresh = component_.get_player_time_refresh(key);

        BOOST_CHECK(player_time_refresh.get_time_refresh_type() == key.get_time_refresh_type());
        BOOST_CHECK_EQUAL(player_time_refresh.get_parameter(), key.get_parameter());
        BOOST_CHECK_EQUAL(player_time_refresh.get_time_id(), key.get_time_id());
        BOOST_CHECK(player_time_refresh.get_component() == celeritas::player_time_refresh::component_container{celeritas::player_component_type::mock});
        BOOST_CHECK_GE(celeritas::time_helper::get_current_milliseconds(), player_time_refresh.get_last_refresh_time());
    }

BOOST_AUTO_TEST_SUITE_END()
