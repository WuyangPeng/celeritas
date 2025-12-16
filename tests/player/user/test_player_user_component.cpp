#include "config/database_type.h"
#include "player/user/player_user_component.h"
#include "database/database_pool_base.h"
#include "database/generated/mysql/player/user.h"
#include "player/component/player_state.h"
#include "player/mock_database_pool.h"
#include "player/mock_player_state.h"

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/test/unit_test.hpp>

namespace
{
    // 用于设置测试环境的 Fixture
    struct player_user_component_fixture
    {
        player_user_component_fixture()
            : io_context_{},
              mock_pool_{ std::make_shared<celeritas::mock_database_pool>() },
              // 使用一个有效的构造函数来创建 user 对象
              test_user_{ celeritas::database_type::mysql, 12345 },
              mock_player_state_{ io_context_ }
        {
        }

        boost::asio::io_context io_context_;
        std::shared_ptr<celeritas::mock_database_pool> mock_pool_;
        celeritas::user test_user_;
        celeritas::mock_player_state mock_player_state_;
    };
}

BOOST_FIXTURE_TEST_SUITE(player_user_component_suite, player_user_component_fixture)

    BOOST_AUTO_TEST_CASE(test_save_db_with_modifications)
    {
        // 修改 user 对象以触发保存
        test_user_.set_game_server_id("new_server");
        test_user_.set_user_id(10000);
        BOOST_CHECK(test_user_.is_modify());

        celeritas::player_user_component component{ test_user_, &mock_player_state_ };
        component.set_mock_database_pool(mock_pool_);

        BOOST_CHECK_EQUAL(component.get_game_server_id(), test_user_.get_game_server_id());
        BOOST_CHECK_EQUAL(component.get_user_id(), test_user_.get_user_id());

        // 调用并执行协程
        boost::asio::co_spawn(io_context_, component.save_db(), boost::asio::detached);
        io_context_.run();

        // 验证 execute_changes 方法是否被调用了一次
        BOOST_CHECK_EQUAL(mock_pool_->get_execute_changes_call_count(), 1);
        // 验证修改标记已被清除
        BOOST_CHECK(!component.is_modify());
    }

    BOOST_AUTO_TEST_CASE(test_save_db_without_modifications)
    {
        test_user_.clear_modify();
        BOOST_CHECK(!test_user_.is_modify());

        celeritas::player_user_component component{ test_user_, &mock_player_state_ };
        component.set_mock_database_pool(mock_pool_);

        // 调用并执行协程
        boost::asio::co_spawn(io_context_, component.save_db(), boost::asio::detached);
        io_context_.run();

        // 验证 execute_changes 方法完全没有被调用
        BOOST_CHECK_EQUAL(mock_pool_->get_execute_changes_call_count(), 0);
    }

    BOOST_AUTO_TEST_CASE(test_on_db_analysis)
    {
        // 先将 overload_db 设为 true
        test_user_.set_overload_db(true);
        BOOST_CHECK(test_user_.is_overload_db());

        celeritas::player_user_component component{ test_user_, &mock_player_state_ };
        component.set_mock_database_pool(mock_pool_);

        // 调用并执行协程
        boost::asio::co_spawn(io_context_, component.on_db_analysis(), boost::asio::detached);
        io_context_.run();

        // 验证 overload_db 已被设为 false
        BOOST_CHECK(!component.is_overload_db());
    }

BOOST_AUTO_TEST_SUITE_END()
