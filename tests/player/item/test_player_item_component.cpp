#include "config/app_config.h"
#include "config/game_config/container_config.tpp"
#include "config/game_config/game_config.h"
#include "config/game_config/game_tables.h"
#include "config/game_config/item_config.h"
#include "database/database_pool_manager.h"
#include "player/item/player_item_component.h"
#include "player/mock/mock_database_pool.h"
#include "player/mock/mock_player_state.h"

#include <boost/asio/io_context.hpp>
#include <boost/test/unit_test.hpp>

namespace
{
    struct player_item_component_fixture
    {
        player_item_component_fixture()
            : io_context_{},
              mock_player_state_{ io_context_ },
              mock_pool_{ std::make_shared<celeritas::mock_database_pool>() },
              component_{ std::make_shared<celeritas::player_item_component>(&mock_player_state_) },
              app_config_{ std::make_shared<celeritas::app_config>() }
        {
            celeritas::database_pool_manager::get_instance().set_mock_pool(mock_pool_);

            init_item_config();
        }

        static void init_item_config()
        {
            const auto game_tables = std::make_shared<celeritas::game_tables>();
            const auto item_config_container = std::make_shared<celeritas::game_tables::item_container_config>();

            item_config_container->add_config(std::make_shared<celeritas::item_config>(1001, celeritas::config::item_type::custom, 100, false));
            item_config_container->add_config(std::make_shared<celeritas::item_config>(1002, celeritas::config::item_type::equipment, 1, false));
            item_config_container->add_config(std::make_shared<celeritas::item_config>(1003, celeritas::config::item_type::equipment, 1, false));

            game_tables->set_item_config(item_config_container);
            celeritas::game_config::get_instance().set_game_tables(game_tables);
        }

        void run_io_context()
        {
            io_context_.restart();
            io_context_.run();
        }

        void load_component()
        {
            boost::asio::co_spawn(io_context_, component_->on_load_db(), boost::asio::detached);
            run_io_context();
        }

        boost::asio::io_context io_context_;
        celeritas::mock_player_state mock_player_state_;
        std::shared_ptr<celeritas::mock_database_pool> mock_pool_;
        std::shared_ptr<celeritas::player_item_component> component_;
        std::shared_ptr<celeritas::app_config> app_config_;
    };
}

BOOST_FIXTURE_TEST_SUITE(player_item_component_suite, player_item_component_fixture)

    BOOST_AUTO_TEST_CASE(test_get_player_component_type)
    {
        BOOST_CHECK_EQUAL(static_cast<int>(component_->get_player_component_type()), static_cast<int>(celeritas::player_component_type::item));
        BOOST_CHECK_EQUAL(static_cast<int>(celeritas::player_item_component::get_player_component_type()), static_cast<int>(celeritas::player_component_type::item));
    }

    BOOST_AUTO_TEST_CASE(test_on_load_db)
    {
        load_component();

        BOOST_CHECK(!component_->is_modify());
        BOOST_CHECK_EQUAL(component_->get_count(1003), 111);
    }

    BOOST_AUTO_TEST_CASE(test_change_item)
    {
        load_component();

        component_->change_item(app_config_, 1001, 50);
        BOOST_CHECK_EQUAL(component_->get_count(1001), 50);
        BOOST_CHECK(component_->is_modify());

        component_->change_item(app_config_, 1001, 60);
        BOOST_CHECK_EQUAL(component_->get_count(1001), 110);

        component_->change_item(app_config_, 1001, -10);
        BOOST_CHECK_EQUAL(component_->get_count(1001), 100);

        component_->change_item(app_config_, 1001, -100);
        BOOST_CHECK_EQUAL(component_->get_count(1001), 0);
    }

    BOOST_AUTO_TEST_CASE(test_can_consume_item)
    {
        load_component();

        component_->change_item(app_config_, 1001, 100);

        BOOST_CHECK(component_->can_consume_item(1001, 50));
        BOOST_CHECK(component_->can_consume_item(1001, 100));
        BOOST_CHECK(!component_->can_consume_item(1001, 101));
        BOOST_CHECK(!component_->can_consume_item(1002, 1));
    }

    BOOST_AUTO_TEST_CASE(test_item_container_operations)
    {
        load_component();

        celeritas::item_container container{};
        container.add_item_info(1001, 50);
        container.add_item_info(1002, 1);

        component_->change_item(app_config_, container);
        BOOST_CHECK_EQUAL(component_->get_count(1001), 50);
        BOOST_CHECK_EQUAL(component_->get_count(1002), 1);

        BOOST_CHECK(component_->can_consume_item(container));

        celeritas::item_container consume_container;
        consume_container.add_item_info(1001, 51);
        BOOST_CHECK(!component_->can_consume_item(consume_container));
    }

    BOOST_AUTO_TEST_CASE(test_save_db)
    {
        load_component();

        BOOST_CHECK(!component_->is_modify());

        component_->change_item(app_config_, 1001, 10);
        BOOST_CHECK(component_->is_modify());

        boost::asio::co_spawn(io_context_, component_->save_db(), boost::asio::detached);
        run_io_context();

        BOOST_CHECK(!component_->is_modify());
    }

BOOST_AUTO_TEST_SUITE_END()
