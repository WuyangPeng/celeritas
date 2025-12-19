#include "config/game_config/container_config.tpp"
#include "config/game_config/game_config.h"
#include "config/game_config/game_tables.h"
#include "config/game_config/red_dot_config.h"
#include "config/game_config/red_dot_status_type.h"
#include "config/game_config/red_dot_type.h"
#include "player/mock_database_pool.h"
#include "player/mock_player_state.h"
#include "player/red_dot/player_red_dot_component.h"

#include <boost/asio/io_context.hpp>
#include <boost/test/unit_test.hpp>

namespace
{
    struct player_red_dot_component_fixture
    {
        player_red_dot_component_fixture()
            : io_context_{},
              mock_player_state_{ io_context_ },
              mock_pool_{ std::make_shared<celeritas::mock_database_pool>() },
              component_{ std::make_shared<celeritas::player_red_dot_component>(&mock_player_state_) }
        {
            component_->set_mock_database_pool(mock_pool_);

            init_game_config();
        }

        void run_io_context()
        {
            io_context_.restart();
            io_context_.run();
        }

        static void init_game_config()
        {
            const auto red_dot = std::make_shared<celeritas::container_config<celeritas::red_dot_config, celeritas::red_dot_type> >();

            const auto red_dot_config = std::make_shared<celeritas::red_dot_config>(celeritas::red_dot_type::role, "test", celeritas::red_dot_type::null, celeritas::red_dot_status_type::sum, true);

            red_dot->add_config(red_dot_config);

            const auto game_tables = std::make_shared<celeritas::game_tables>();
            game_tables->set_red_dot_config(red_dot);

            celeritas::game_config::get_instance().set_game_tables(game_tables);
        }

        boost::asio::io_context io_context_;
        celeritas::mock_player_state mock_player_state_;
        std::shared_ptr<celeritas::mock_database_pool> mock_pool_;
        std::shared_ptr<celeritas::player_red_dot_component> component_;
    };
}

BOOST_FIXTURE_TEST_SUITE(player_red_dot_component_suite, player_red_dot_component_fixture)

    BOOST_AUTO_TEST_CASE(test_get_player_component_type)
    {
        BOOST_CHECK_EQUAL(static_cast<int>(component_->get_player_component_type()), static_cast<int>(celeritas::player_component_type::red_dot));
        BOOST_CHECK_EQUAL(static_cast<int>(celeritas::player_red_dot_component::get_player_component_type()), static_cast<int>(celeritas::player_component_type::red_dot));
    }

    BOOST_AUTO_TEST_CASE(test_on_load_db)
    {
        boost::asio::co_spawn(io_context_, component_->on_load_db(), boost::asio::detached);
        run_io_context();

        BOOST_CHECK(!component_->is_modify());
    }

    BOOST_AUTO_TEST_CASE(test_on_save_db)
    {
        boost::asio::co_spawn(io_context_, component_->on_load_db(), boost::asio::detached);
        run_io_context();

        component_->add_red_dot(celeritas::red_dot_type::role);

        BOOST_CHECK(component_->is_modify());

        boost::asio::co_spawn(io_context_, component_->save_db(), boost::asio::detached);
        run_io_context();

        BOOST_CHECK(!component_->is_modify());
    }

    BOOST_AUTO_TEST_CASE(test_on_dependencies_ready)
    {
        boost::asio::co_spawn(io_context_, component_->on_load_db(), boost::asio::detached);
        run_io_context();

        boost::asio::co_spawn(io_context_, component_->on_dependencies_ready(), boost::asio::detached);
        run_io_context();

        BOOST_CHECK(component_->is_modify());
    }

    BOOST_AUTO_TEST_CASE(test_add_red_dot)
    {
        boost::asio::co_spawn(io_context_, component_->on_load_db(), boost::asio::detached);
        run_io_context();

        component_->add_red_dot(celeritas::red_dot_type::role);
        BOOST_CHECK_EQUAL(component_->get_red_dot_value(celeritas::red_dot_type::role), 1);
    }

    BOOST_AUTO_TEST_CASE(test_add_red_dot_with_value)
    {
        boost::asio::co_spawn(io_context_, component_->on_load_db(), boost::asio::detached);
        run_io_context();

        component_->add_red_dot(celeritas::red_dot_type::role, 5);
        BOOST_CHECK_EQUAL(component_->get_red_dot_value(celeritas::red_dot_type::role), 5);
    }

    BOOST_AUTO_TEST_CASE(test_reduce_red_dot)
    {
        boost::asio::co_spawn(io_context_, component_->on_load_db(), boost::asio::detached);
        run_io_context();

        component_->add_red_dot(celeritas::red_dot_type::role, 3);
        BOOST_CHECK_EQUAL(component_->get_red_dot_value(celeritas::red_dot_type::role), 3);

        component_->reduce_red_dot(celeritas::red_dot_type::role);
        BOOST_CHECK_EQUAL(component_->get_red_dot_value(celeritas::red_dot_type::role), 2);
    }

    BOOST_AUTO_TEST_CASE(test_reduce_red_dot_with_value)
    {
        boost::asio::co_spawn(io_context_, component_->on_load_db(), boost::asio::detached);
        run_io_context();

        component_->add_red_dot(celeritas::red_dot_type::role, 3);
        component_->reduce_red_dot(celeritas::red_dot_type::role, 2);
        BOOST_CHECK_EQUAL(component_->get_red_dot_value(celeritas::red_dot_type::role), 1);
    }

    BOOST_AUTO_TEST_CASE(test_change_red_dot)
    {
        boost::asio::co_spawn(io_context_, component_->on_load_db(), boost::asio::detached);
        run_io_context();

        component_->add_red_dot(celeritas::red_dot_type::role);
        BOOST_CHECK_EQUAL(component_->get_red_dot_value(celeritas::red_dot_type::role), 1);

        component_->change_red_dot(celeritas::red_dot_type::role);
        BOOST_CHECK_EQUAL(component_->get_red_dot_value(celeritas::red_dot_type::role), 0);
    }

BOOST_AUTO_TEST_SUITE_END()
