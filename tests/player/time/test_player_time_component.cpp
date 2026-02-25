#include "common/core/time_helper.h"
#include "database/document/player_time_refresh.h"
#include "fixture/player_time_component_fixture.h"
#include "player/time/time_refresh_type.h"

#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_SUITE(player_time_component_suite, celeritas::player_time_component_fixture)

    BOOST_AUTO_TEST_CASE(test_initial_state)
    {
        run([this]() -> void_awaitable_type {
            co_await get_component()->on_load_db();
            BOOST_CHECK(!get_component()->is_modify());

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_register_and_remove_timer)
    {
        const celeritas::player_time_refresh_key key{ celeritas::time_refresh_type::daily, 1, 1 };
        constexpr auto component_type = celeritas::player_component_type::user;

        run([this]() -> void_awaitable_type {
            co_await get_component()->on_load_db();
            BOOST_CHECK(!get_component()->is_modify());

            set_test_end(true);
        });

        get_component()->register_timer(component_type, key);
        BOOST_CHECK(get_component()->is_modify());

        // 停掉定时器，不然测试会卡死
        get_component()->stop_timer();

        run([this]() -> void_awaitable_type {
            set_test_end(false);

            co_await get_component()->save_db();

            set_test_end(true);
        });

        BOOST_CHECK(!get_component()->is_modify());

        get_component()->remove_timer(component_type, key);
        BOOST_CHECK(get_component()->is_modify());
    }

    BOOST_AUTO_TEST_CASE(test_save_db_with_modifications)
    {
        run([this]() -> void_awaitable_type {
            co_await get_component()->on_load_db();

            set_test_end(true);
        });

        const celeritas::player_time_refresh_key key{ celeritas::time_refresh_type::daily, 1, 1 };
        get_component()->register_timer(celeritas::player_component_type::user, key);
        BOOST_CHECK(get_component()->is_modify());

        // 停掉定时器，不然测试会卡死
        get_component()->stop_timer();

        run([this]() -> void_awaitable_type {
            set_test_end(false);

            co_await get_component()->save_db();

            set_test_end(true);
        });

        BOOST_CHECK_EQUAL(get_mock_pool()->get_execute_changes_call_count(), 1);
        BOOST_CHECK(!get_component()->is_modify());
    }

    BOOST_AUTO_TEST_CASE(test_save_db_without_modifications)
    {
        run([this]() -> void_awaitable_type {
            co_await get_component()->on_load_db();

            BOOST_CHECK(!get_component()->is_modify());

            co_await get_component()->save_db();

            BOOST_CHECK_EQUAL(get_mock_pool()->get_execute_changes_call_count(), 0);
            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_on_load_db)
    {
        run([this]() -> void_awaitable_type {
            const celeritas::player_time_refresh_key key{ celeritas::time_refresh_type::weekly, 123, 456 };

            co_await get_component()->on_load_db();

            const auto player_time_refresh = get_component()->get_player_time_refresh(key);

            BOOST_CHECK(player_time_refresh.get_time_refresh_type() == key.get_time_refresh_type());
            BOOST_CHECK_EQUAL(player_time_refresh.get_parameter(), key.get_parameter());
            BOOST_CHECK_EQUAL(player_time_refresh.get_time_id(), key.get_time_id());
            BOOST_CHECK(player_time_refresh.get_component() == celeritas::player_time_refresh::component_container{celeritas::player_component_type::finish});
            BOOST_CHECK_GE(celeritas::time_helper::get_current_milliseconds(), player_time_refresh.get_last_refresh_time());

            set_test_end(true);
        });
    }

    BOOST_AUTO_TEST_CASE(test_on_dependencies_ready)
    {
        run([this]() -> void_awaitable_type {
            co_await get_component()->on_load_db();
            set_test_end(true);
        });

        const celeritas::player_time_refresh_key key{ celeritas::time_refresh_type::interval_duration, 0, 1 };
        get_component()->register_timer(celeritas::player_component_type::mock, key);

        // 停掉定时器，不然测试会卡死
        get_component()->stop_timer();

        BOOST_CHECK_EQUAL(get_mock_player_component()->get_time_callback_is_login(), 0);
        BOOST_CHECK_EQUAL(get_mock_player_component()->get_time_callback_is_not_login(), 0);

        spawn([this]() -> void_awaitable_type {
            co_await get_component()->on_dependencies_ready();
        });

        run_io_context_two_times();

        BOOST_CHECK_EQUAL(get_mock_player_component()->get_time_callback_is_login(), 1);
        BOOST_CHECK_EQUAL(get_mock_player_component()->get_time_callback_is_not_login(), 0);
    }

    BOOST_AUTO_TEST_CASE(test_time_callback_with_args)
    {
        run([this]() -> void_awaitable_type {
            co_await get_component()->on_load_db();
            set_test_end(true);
        });

        const celeritas::player_time_refresh_key key{ celeritas::time_refresh_type::interval_duration, 0, 2 };
        get_component()->register_timer(celeritas::player_component_type::mock, key);

        // 停掉定时器，不然测试会卡死
        get_component()->stop_timer();

        BOOST_CHECK_EQUAL(get_mock_player_component()->get_time_callback_is_login(), 0);
        BOOST_CHECK_EQUAL(get_mock_player_component()->get_time_callback_is_not_login(), 0);

        run([this,&key]() -> void_awaitable_type {
            set_test_end(false);
            co_await get_component()->time_callback(key, true);
            set_test_end(true);
        });

        BOOST_CHECK_EQUAL(get_mock_player_component()->get_time_callback_is_login(), 1);
        BOOST_CHECK_EQUAL(get_mock_player_component()->get_time_callback_is_not_login(), 0);

        run([this,&key]() -> void_awaitable_type {
            set_test_end(false);
            co_await get_component()->time_callback(key, false);
            set_test_end(true);
        });

        BOOST_CHECK_EQUAL(get_mock_player_component()->get_time_callback_is_login(), 1);
        BOOST_CHECK_EQUAL(get_mock_player_component()->get_time_callback_is_not_login(), 1);
    }

    BOOST_AUTO_TEST_CASE(test_time_callback_no_args)
    {
        run([this]() -> void_awaitable_type {
            co_await get_component()->on_load_db();
            set_test_end(true);
        });

        const celeritas::player_time_refresh_key key{ celeritas::time_refresh_type::interval_duration, 0, 2 };
        get_component()->register_timer(celeritas::player_component_type::mock, key);

        // 停掉定时器，不然测试会卡死
        get_component()->stop_timer();

        BOOST_CHECK_EQUAL(get_mock_player_component()->get_time_callback_is_login(), 0);
        BOOST_CHECK_EQUAL(get_mock_player_component()->get_time_callback_is_not_login(), 0);

        spawn([this]() -> void_awaitable_type {
            co_await get_component()->time_callback();
        });

        run_io_context_two_times();

        BOOST_CHECK_EQUAL(get_mock_player_component()->get_time_callback_is_login(), 0);
        BOOST_CHECK_EQUAL(get_mock_player_component()->get_time_callback_is_not_login(), 1);
    }

BOOST_AUTO_TEST_SUITE_END()
