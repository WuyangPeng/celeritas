#include "config/aggregate/app_config.h"
#include "fixture/player_user_component_fixture.h"
#include "player/component/player_state.h"
#include "player/user/player_user_component.h"

#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_SUITE(player_user_component_suite, celeritas::player_user_component_fixture)

    BOOST_AUTO_TEST_CASE(test_save_db_with_modifications)
    {
        auto& test_user = get_test_user();

        // 修改 user 对象以触发保存
        test_user.set_game_server_id("new_server");
        test_user.set_user_id(10000);
        BOOST_CHECK(test_user.is_modify());

        celeritas::player_user_component component{ test_user, &get_mock_player_state() };

        BOOST_CHECK_EQUAL(component.get_game_server_id(), test_user.get_game_server_id());
        BOOST_CHECK_EQUAL(component.get_user_id(), test_user.get_user_id());

        run([this,&component]() -> void_awaitable_type {
            co_await component.save_db();

            set_test_end(true);
        });

        // 验证 execute_changes 方法是否被调用了一次
        BOOST_CHECK_EQUAL(get_mock_pool()->get_execute_changes_call_count(), 1);
        // 验证修改标记已被清除
        BOOST_CHECK(!component.is_modify());
    }

    BOOST_AUTO_TEST_CASE(test_save_db_without_modifications)
    {
        auto& test_user = get_test_user();

        test_user.clear_modify();
        BOOST_CHECK(!test_user.is_modify());

        celeritas::player_user_component component{ test_user, &get_mock_player_state() };

        run([this,&component]() -> void_awaitable_type {
            co_await component.save_db();

            set_test_end(true);
        });

        // 验证 execute_changes 方法完全没有被调用
        BOOST_CHECK_EQUAL(get_mock_pool()->get_execute_changes_call_count(), 0);
    }

    BOOST_AUTO_TEST_CASE(test_on_db_analysis)
    {
        auto& test_user = get_test_user();

        // 先将 overload_db 设为 true
        test_user.set_overload_db(true);
        BOOST_CHECK(test_user.is_overload_db());

        celeritas::player_user_component component{ test_user, &get_mock_player_state() };

        run([this,&component]() -> void_awaitable_type {
            co_await component.on_db_analysis(std::make_shared<celeritas::app_config>());

            set_test_end(true);
        });

        // 验证 overload_db 已被设为 false
        BOOST_CHECK(!component.is_overload_db());
    }

BOOST_AUTO_TEST_SUITE_END()
