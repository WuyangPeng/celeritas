#include "config/aggregate/app_config.h"
#include "database/pool/database_pool_manager.h"
#include "message/basic/game_error_type.h"
#include "message/parameters/protobuf_handle_parameter.h"
#include "player/component/player_manager.h"
#include "player/component/player_state.h"
#include "player/login/service_login.h"
#include "player/mock/mock_application_loader.h"
#include "player/mock/mock_database_pool.h"
#include "player/mock/mock_resource_loader.h"
#include "player/mock/mock_session.h"
#include "player/time/player_time_component.h"
#include "proto/celeritas.pb.h"

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/test/unit_test.hpp>

namespace
{
    struct service_login_fixture
    {
        service_login_fixture()
            : io_context_{},
              mock_pool_{ std::make_shared<celeritas::mock_database_pool>() },
              mock_session_{ std::make_shared<celeritas::mock_session>(boost::asio::make_strand(io_context_)) },
              mock_resource_loader_{ std::make_shared<celeritas::mock_resource_loader>() },
              mock_application_loader_{ std::make_shared<celeritas::mock_application_loader>() }
        {
            celeritas::database_pool_manager::get_instance().set_mock_pool(mock_pool_);

            celeritas::header header{};
            auto request_message = std::make_shared<celeritas::proto::celeritas>();
            parameter_ = std::make_shared<celeritas::protobuf_handle_parameter>(header,
                                                                                request_message,
                                                                                mock_session_,
                                                                                mock_resource_loader_,
                                                                                mock_application_loader_);
        }

        ~service_login_fixture()
        {
            celeritas::database_pool_manager::get_instance().set_mock_pool(nullptr);
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
        std::shared_ptr<celeritas::mock_database_pool> mock_pool_;
        std::shared_ptr<celeritas::mock_session> mock_session_;
        std::shared_ptr<celeritas::mock_resource_loader> mock_resource_loader_;
        std::shared_ptr<celeritas::mock_application_loader> mock_application_loader_;
        std::shared_ptr<celeritas::protobuf_handle_parameter> parameter_;
    };
}

BOOST_FIXTURE_TEST_SUITE(service_login_suite, service_login_fixture)

    BOOST_AUTO_TEST_CASE(test_login_existing_user_success)
    {
        celeritas::proto::service::service_login_request login_request{};
        login_request.set_account_id(111);
        login_request.set_game_server_id("test_server");
        login_request.set_new_account(false);

        const celeritas::service_login service{ parameter_, login_request };

        // 模拟 user 查询成功
        mock_pool_->set_select_one_result(true);

        boost::asio::co_spawn(io_context_,
                              [&]() -> boost::asio::awaitable<void> {
                                  const auto user_id = co_await service.send_message();
                                  const auto player_state = celeritas::player_manager::get_instance().get_player(user_id);
                                  player_state->get_component<celeritas::player_time_component>()->stop_timer();
                              },
                              boost::asio::detached);

        run_io_context_two_times();

        // 应该查询到了 user
        BOOST_CHECK_GT(mock_pool_->get_select_one_call_count(), 1);

        // 应该发送了成功消息
        // 我们可以通过 mock_session_ 检查最后发送的消息
        BOOST_CHECK(mock_session_->get_code() == celeritas::game_error_type::success);

        boost::asio::co_spawn(io_context_,
                              [&]() -> boost::asio::awaitable<void> {
                                  co_await celeritas::player_manager::get_instance().clear();
                              },
                              boost::asio::detached);

        run_io_context_two_times();
    }

    BOOST_AUTO_TEST_CASE(test_login_new_user_creation_success)
    {
        celeritas::proto::service::service_login_request login_request{};
        login_request.set_account_id(111);
        login_request.set_game_server_id("test_server");
        login_request.set_new_account(false);

        const celeritas::service_login service{ parameter_, login_request };

        // 模拟 user 查询失败 (用户未找到)
        mock_pool_->set_select_one_result(false);
        // 模拟创建 user 成功 (执行变更)
        mock_pool_->set_execute_changes_result(true);

        boost::asio::co_spawn(io_context_,
                              [&]() -> boost::asio::awaitable<void> {
                                  co_await service.send_message();
                              },
                              boost::asio::detached);

        run_io_context();

        BOOST_CHECK_GT(mock_pool_->get_select_one_call_count(), 1);
        BOOST_CHECK_EQUAL(mock_pool_->get_execute_changes_call_count(), 1);

        BOOST_CHECK(mock_session_->get_code() == celeritas::game_error_type::success);

        boost::asio::co_spawn(io_context_,
                              [&]() -> boost::asio::awaitable<void> {
                                  co_await celeritas::player_manager::get_instance().clear();
                              },
                              boost::asio::detached);

        run_io_context_two_times();
    }

    BOOST_AUTO_TEST_CASE(test_login_new_account_creation_failure)
    {
        celeritas::proto::service::service_login_request login_request{};
        login_request.set_account_id(111);
        login_request.set_new_account(true); // 请求创建新账号

        const celeritas::service_login service{ parameter_, login_request };

        // 模拟 redis account 查询失败 (导致 create_account 失败)
        mock_pool_->set_select_one_result(false);

        boost::asio::co_spawn(io_context_,
                              [&]() -> boost::asio::awaitable<void> {
                                  co_await service.send_message();
                              },
                              boost::asio::detached);

        run_io_context();

        // 应该在 create_account 步骤失败
        BOOST_CHECK(mock_session_->get_code() == celeritas::game_error_type::create_account_error);
    }

BOOST_AUTO_TEST_SUITE_END()
