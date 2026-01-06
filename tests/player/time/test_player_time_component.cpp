#include "common/core/time_helper.h"
#include "database/pool/database_pool_manager.h"
#include "database/document/player_time_refresh.h"
#include "player/mock/mock_database_pool.h"
#include "player/mock/mock_player_component.h"
#include "player/mock/mock_player_state.h"
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
              mock_player_state_{ boost::asio::make_strand(io_context_) },
              mock_pool_{ std::make_shared<celeritas::mock_database_pool>() },
              component_{ std::make_shared<celeritas::player_time_component>(&mock_player_state_) },
              mock_player_component_{ std::make_shared<celeritas::mock_player_component>(&mock_player_state_) }
        {
            celeritas::database_pool_manager::get_instance().set_mock_pool(mock_pool_);

            mock_player_state_.set_mock_player_component(mock_player_component_);
        }

        void run_io_context()
        {
            io_context_.restart();
            io_context_.run();
        }

        void run_io_context_two_times()
        {
            // 由于存在定时器，避免卡死
            io_context_.restart();
            io_context_.run_one();
            io_context_.run_one();
        }

        boost::asio::io_context io_context_;
        celeritas::mock_player_state mock_player_state_;
        std::shared_ptr<celeritas::mock_database_pool> mock_pool_;
        std::shared_ptr<celeritas::player_time_component> component_;
        std::shared_ptr<celeritas::mock_player_component> mock_player_component_;
    };
}

BOOST_FIXTURE_TEST_SUITE(player_time_component_suite, player_time_component_fixture)

    BOOST_AUTO_TEST_CASE(test_initial_state)
    {
        boost::asio::co_spawn(io_context_, component_->on_load_db(), boost::asio::detached);
        run_io_context();

        BOOST_CHECK(!component_->is_modify());
    }

    BOOST_AUTO_TEST_CASE(test_register_and_remove_timer)
    {
        boost::asio::co_spawn(io_context_, component_->on_load_db(), boost::asio::detached);
        run_io_context();

        const celeritas::player_time_refresh_key key{ celeritas::time_refresh_type::daily, 1, 1 };
        constexpr auto component_type = celeritas::player_component_type::user;

        BOOST_CHECK(!component_->is_modify());

        component_->register_timer(component_type, key);
        BOOST_CHECK(component_->is_modify());

        // 停掉定时器，不然测试会卡死
        component_->stop_timer();

        boost::asio::co_spawn(io_context_, component_->save_db(), boost::asio::detached);
        run_io_context();

        BOOST_CHECK(!component_->is_modify());

        component_->remove_timer(component_type, key);
        BOOST_CHECK(component_->is_modify());
    }

    BOOST_AUTO_TEST_CASE(test_save_db_with_modifications)
    {
        boost::asio::co_spawn(io_context_, component_->on_load_db(), boost::asio::detached);
        run_io_context();

        const celeritas::player_time_refresh_key key{ celeritas::time_refresh_type::daily, 1, 1 };
        component_->register_timer(celeritas::player_component_type::user, key);
        BOOST_CHECK(component_->is_modify());

        // 停掉定时器，不然测试会卡死
        component_->stop_timer();

        boost::asio::co_spawn(io_context_, component_->save_db(), boost::asio::detached);
        run_io_context();

        BOOST_CHECK_EQUAL(mock_pool_->get_execute_changes_call_count(), 1);
        BOOST_CHECK(!component_->is_modify());
    }

    BOOST_AUTO_TEST_CASE(test_save_db_without_modifications)
    {
        boost::asio::co_spawn(io_context_, component_->on_load_db(), boost::asio::detached);
        run_io_context();

        BOOST_CHECK(!component_->is_modify());

        boost::asio::co_spawn(io_context_, component_->save_db(), boost::asio::detached);
        run_io_context();

        BOOST_CHECK_EQUAL(mock_pool_->get_execute_changes_call_count(), 0);
    }

    BOOST_AUTO_TEST_CASE(test_on_load_db)
    {
        const celeritas::player_time_refresh_key key{ celeritas::time_refresh_type::weekly, 123, 456 };

        boost::asio::co_spawn(io_context_, component_->on_load_db(), boost::asio::detached);
        run_io_context();

        const auto player_time_refresh = component_->get_player_time_refresh(key);

        BOOST_CHECK(player_time_refresh.get_time_refresh_type() == key.get_time_refresh_type());
        BOOST_CHECK_EQUAL(player_time_refresh.get_parameter(), key.get_parameter());
        BOOST_CHECK_EQUAL(player_time_refresh.get_time_id(), key.get_time_id());
        BOOST_CHECK(player_time_refresh.get_component() == celeritas::player_time_refresh::component_container{celeritas::player_component_type::finish});
        BOOST_CHECK_GE(celeritas::time_helper::get_current_milliseconds(), player_time_refresh.get_last_refresh_time());
    }

    BOOST_AUTO_TEST_CASE(test_on_dependencies_ready)
    {
        boost::asio::co_spawn(io_context_, component_->on_load_db(), boost::asio::detached);
        run_io_context();

        const celeritas::player_time_refresh_key key{ celeritas::time_refresh_type::interval_duration, 0, 1 };
        component_->register_timer(celeritas::player_component_type::mock, key);

        // 停掉定时器，不然测试会卡死
        component_->stop_timer();

        BOOST_CHECK_EQUAL(mock_player_component_->get_time_callback_is_login(), 0);
        BOOST_CHECK_EQUAL(mock_player_component_->get_time_callback_is_not_login(), 0);

        boost::asio::co_spawn(io_context_, component_->on_dependencies_ready(), boost::asio::detached);
        run_io_context_two_times();

        BOOST_CHECK_EQUAL(mock_player_component_->get_time_callback_is_login(), 1);
        BOOST_CHECK_EQUAL(mock_player_component_->get_time_callback_is_not_login(), 0);
    }

    BOOST_AUTO_TEST_CASE(test_time_callback_with_args)
    {
        boost::asio::co_spawn(io_context_, component_->on_load_db(), boost::asio::detached);
        run_io_context();

        const celeritas::player_time_refresh_key key{ celeritas::time_refresh_type::interval_duration, 0, 2 };
        component_->register_timer(celeritas::player_component_type::mock, key);

        // 停掉定时器，不然测试会卡死
        component_->stop_timer();

        BOOST_CHECK_EQUAL(mock_player_component_->get_time_callback_is_login(), 0);
        BOOST_CHECK_EQUAL(mock_player_component_->get_time_callback_is_not_login(), 0);

        boost::asio::co_spawn(io_context_, component_->time_callback(key, true), boost::asio::detached);
        run_io_context();

        BOOST_CHECK_EQUAL(mock_player_component_->get_time_callback_is_login(), 1);
        BOOST_CHECK_EQUAL(mock_player_component_->get_time_callback_is_not_login(), 0);

        boost::asio::co_spawn(io_context_, component_->time_callback(key, false), boost::asio::detached);
        run_io_context();

        BOOST_CHECK_EQUAL(mock_player_component_->get_time_callback_is_login(), 1);
        BOOST_CHECK_EQUAL(mock_player_component_->get_time_callback_is_not_login(), 1);
    }

    BOOST_AUTO_TEST_CASE(test_time_callback_no_args)
    {
        boost::asio::co_spawn(io_context_, component_->on_load_db(), boost::asio::detached);
        run_io_context();

        const celeritas::player_time_refresh_key key{ celeritas::time_refresh_type::interval_duration, 0, 2 };
        component_->register_timer(celeritas::player_component_type::mock, key);

        // 停掉定时器，不然测试会卡死
        component_->stop_timer();

        BOOST_CHECK_EQUAL(mock_player_component_->get_time_callback_is_login(), 0);
        BOOST_CHECK_EQUAL(mock_player_component_->get_time_callback_is_not_login(), 0);

        boost::asio::co_spawn(io_context_, component_->time_callback(), boost::asio::detached);
        run_io_context_two_times();

        BOOST_CHECK_EQUAL(mock_player_component_->get_time_callback_is_login(), 0);
        BOOST_CHECK_EQUAL(mock_player_component_->get_time_callback_is_not_login(), 1);
    }

BOOST_AUTO_TEST_SUITE_END()
