#include "database/pool/database_pool_manager.h"
#include "player/mail/player_mail_component.h"
#include "player/mock/mock_database_pool.h"
#include "player/mock/mock_player_state.h"

#include <boost/asio/io_context.hpp>
#include <boost/test/unit_test.hpp>

namespace
{
    struct player_mail_component_fixture
    {
        player_mail_component_fixture()
            : io_context_{},
              mock_player_state_{ boost::asio::make_strand(io_context_) },
              mock_pool_{ std::make_shared<celeritas::mock_database_pool>() },
              component_{ std::make_shared<celeritas::player_mail_component>(&mock_player_state_) }
        {
            celeritas::database_pool_manager::get_instance().set_mock_pool(mock_pool_);
        }

        void run_io_context()
        {
            io_context_.restart();
            io_context_.run();
        }

        boost::asio::io_context io_context_;
        celeritas::mock_player_state mock_player_state_;
        std::shared_ptr<celeritas::mock_database_pool> mock_pool_;
        std::shared_ptr<celeritas::player_mail_component> component_;
    };
}

BOOST_FIXTURE_TEST_SUITE(player_mail_component_suite, player_mail_component_fixture)

    BOOST_AUTO_TEST_CASE(test_get_player_component_type)
    {
        BOOST_CHECK_EQUAL(static_cast<int>(component_->get_player_component_type()), static_cast<int>(celeritas::player_component_type::mail));
        BOOST_CHECK_EQUAL(static_cast<int>(celeritas::player_mail_component::get_player_component_type()), static_cast<int>(celeritas::player_component_type::mail));
    }

    BOOST_AUTO_TEST_CASE(test_on_load_db)
    {
        boost::asio::co_spawn(io_context_, component_->on_load_db(), boost::asio::detached);
        run_io_context();
    }

BOOST_AUTO_TEST_SUITE_END()
