#include "config/aggregate/app_config.h"
#include "database/database_pool_manager.h"
#include "message/parameters/protobuf_handle_parameter.h"
#include "player/login/create_user.h"
#include "player/mock/mock_application_loader.h"
#include "player/mock/mock_database_pool.h"
#include "player/mock/mock_resource_loader.h"
#include "player/mock/mock_session.h"
#include "proto/celeritas.pb.h"

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/test/unit_test.hpp>

namespace
{
    struct create_user_fixture
    {
        create_user_fixture()
            : io_context_{},
              mock_pool_{ std::make_shared<celeritas::mock_database_pool>() },
              mock_session_{ std::make_shared<celeritas::mock_session>(boost::asio::make_strand(io_context_)) },
              mock_resource_loader_{ std::make_shared<celeritas::mock_resource_loader>() },
              mock_application_loader_{ std::make_shared<celeritas::mock_application_loader>() }
        {
            celeritas::database_pool_manager::get_instance().set_mock_pool(mock_pool_);

            celeritas::header header;
            auto request_message = std::make_shared<celeritas::proto::celeritas>();
            parameter_ = std::make_shared<celeritas::protobuf_handle_parameter>(header,
                                                                                request_message,
                                                                                mock_session_,
                                                                                mock_resource_loader_,
                                                                                mock_application_loader_);
        }

        void run_io_context()
        {
            io_context_.restart();
            io_context_.run();
        }

        boost::asio::io_context io_context_;
        std::shared_ptr<celeritas::mock_database_pool> mock_pool_;
        std::shared_ptr<celeritas::mock_session> mock_session_;
        std::shared_ptr<celeritas::mock_resource_loader> mock_resource_loader_;
        std::shared_ptr<celeritas::mock_application_loader> mock_application_loader_;
        std::shared_ptr<celeritas::protobuf_handle_parameter> parameter_;
    };
}

BOOST_FIXTURE_TEST_SUITE(create_user_suite, create_user_fixture)

    BOOST_AUTO_TEST_CASE(test_save_database_success)
    {
        celeritas::proto::service::service_login_request login_request;
        login_request.set_account_id(111);
        login_request.set_game_server_id("test_server");

        celeritas::create_user creator{ parameter_, login_request };

        // 模拟数据库执行成功
        mock_pool_->set_execute_changes_result(true);

        boost::asio::co_spawn(io_context_,
                              [&]() -> boost::asio::awaitable<void> {
                                  auto result = co_await creator.save_database();
                                  BOOST_CHECK(result.has_value());
                                  if (result)
                                  {
                                      BOOST_CHECK_EQUAL(result->get_account_id(), 111);
                                      BOOST_CHECK_EQUAL(result->get_game_server_id(), "test_server");
                                      BOOST_CHECK(result->is_overload_db());
                                  }
                              },
                              boost::asio::detached);

        run_io_context();

        BOOST_CHECK_EQUAL(mock_pool_->get_execute_changes_call_count(), 1);
    }

    BOOST_AUTO_TEST_CASE(test_save_database_failure)
    {
        celeritas::proto::service::service_login_request login_request{};
        login_request.set_account_id(111);
        login_request.set_game_server_id("test_server");

        celeritas::create_user creator{ parameter_, login_request };

        // 模拟数据库执行失败
        mock_pool_->set_execute_changes_result(false);

        boost::asio::co_spawn(io_context_,
                              [&]() -> boost::asio::awaitable<void> {
                                  auto result = co_await creator.save_database();
                                  BOOST_CHECK(!result.has_value());
                              },
                              boost::asio::detached);

        run_io_context();

        BOOST_CHECK_EQUAL(mock_pool_->get_execute_changes_call_count(), 1);
    }

BOOST_AUTO_TEST_SUITE_END()
