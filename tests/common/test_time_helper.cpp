#include "common/common_fwd.h"
#include "common/time_helper.h"

#include <boost/test/unit_test.hpp>

#include <chrono>
#include <thread>

BOOST_AUTO_TEST_SUITE(time_helper_suite)

    struct time_components
    {
        std::chrono::sys_days date;
        std::chrono::milliseconds time_since_midnight;
    };

    [[nodiscard]] time_components get_time_components(const int64_t milliseconds)
    {
        const auto time_point = std::chrono::system_clock::time_point(std::chrono::milliseconds(milliseconds));
        const auto date = std::chrono::floor<std::chrono::days>(time_point);
        const auto time_since_midnight_raw = time_point - date;
        const auto time_since_midnight_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(time_since_midnight_raw);

        return { date, time_since_midnight_milliseconds };
    }

    BOOST_AUTO_TEST_CASE(test_get_current_time)
    {
        const auto seconds1 = celeritas::time_helper::get_current_seconds();
        const auto milliseconds1 = celeritas::time_helper::get_current_milliseconds();

        BOOST_CHECK_GE(milliseconds1 / 1000, seconds1);
        BOOST_CHECK_LE(milliseconds1 / 1000, seconds1 + 1);

        // 休眠一小段时间以确保时间推移
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));

        const auto seconds2 = celeritas::time_helper::get_current_seconds();
        const auto milliseconds2 = celeritas::time_helper::get_current_milliseconds();

        BOOST_CHECK_GE(seconds2, seconds1 + 1);
        BOOST_CHECK_LE(seconds2, seconds1 + 2);

        BOOST_CHECK_GE(milliseconds2, milliseconds1 + 1500);
        BOOST_CHECK_LE(milliseconds2, milliseconds1 + 2000);
    }

    BOOST_AUTO_TEST_CASE(test_get_start_of_day_milliseconds)
    {
        const auto current_milliseconds = celeritas::time_helper::get_current_milliseconds();
        const auto start_of_day_milliseconds = celeritas::time_helper::get_start_of_day_milliseconds();

        BOOST_CHECK_LE(start_of_day_milliseconds, current_milliseconds);
        BOOST_CHECK_GE(current_milliseconds - start_of_day_milliseconds, 0);
        BOOST_CHECK_LT(current_milliseconds - start_of_day_milliseconds, celeritas::day_milliseconds);

        const auto [date, time_since_midnight] = get_time_components(start_of_day_milliseconds);

        BOOST_CHECK_EQUAL(time_since_midnight.count(), 0);
    }

    BOOST_AUTO_TEST_CASE(test_get_milliseconds_with_offset)
    {
        const auto start_of_day_milliseconds = celeritas::time_helper::get_start_of_day_milliseconds();
        const auto yesterday_milliseconds = start_of_day_milliseconds - celeritas::day_milliseconds;
        const auto current_milliseconds = celeritas::time_helper::get_current_milliseconds();

        for (auto i = 0; i < celeritas::day_hour; ++i)
        {
            const auto offset = i * celeritas::hour_milliseconds;
            const auto result = celeritas::time_helper::get_milliseconds_with_offset(offset);

            if (start_of_day_milliseconds + offset < current_milliseconds)
            {
                BOOST_CHECK_EQUAL(result, start_of_day_milliseconds + offset);
            }
            else
            {
                BOOST_CHECK_EQUAL(result, yesterday_milliseconds + offset);
            }
        }
    }

    BOOST_AUTO_TEST_CASE(test_get_start_of_week_milliseconds)
    {
        const auto current_milliseconds = celeritas::time_helper::get_current_milliseconds();
        const auto start_of_week_milliseconds = celeritas::time_helper::get_start_of_week_milliseconds();

        BOOST_CHECK_LE(start_of_week_milliseconds, current_milliseconds);
        BOOST_CHECK_GE(current_milliseconds - start_of_week_milliseconds, 0);
        BOOST_CHECK_LT(current_milliseconds - start_of_week_milliseconds, celeritas::week_milliseconds);

        const auto [date, time_since_midnight] = get_time_components(start_of_week_milliseconds);
        const std::chrono::weekday weekday{ date };

        // 检查时间分量是否为午夜
        BOOST_CHECK_EQUAL(time_since_midnight.count(), 0);
        // 检查是否为星期一
        BOOST_CHECK_EQUAL(weekday.c_encoding(), std::chrono::Monday.c_encoding());
    }

    BOOST_AUTO_TEST_CASE(test_get_start_of_week_milliseconds_with_offset)
    {
        const auto start_of_week_milliseconds = celeritas::time_helper::get_start_of_week_milliseconds();
        const auto start_of_week_offset_zero_milliseconds = celeritas::time_helper::get_start_of_week_milliseconds_with_offset(0);
        BOOST_CHECK_EQUAL(start_of_week_offset_zero_milliseconds, start_of_week_milliseconds);

        const auto current_milliseconds = celeritas::time_helper::get_current_milliseconds();
        const auto last_week_milliseconds = start_of_week_milliseconds - celeritas::week_milliseconds;

        for (auto i = 0; i < celeritas::week; ++i)
        {
            const auto offset = i * celeritas::day_milliseconds;
            const auto result = celeritas::time_helper::get_start_of_week_milliseconds_with_offset(offset);

            if (start_of_week_milliseconds + offset < current_milliseconds)
            {
                BOOST_CHECK_EQUAL(result, start_of_week_milliseconds + offset);
            }
            else
            {
                BOOST_CHECK_EQUAL(result, last_week_milliseconds + offset);
            }
        }
    }

    BOOST_AUTO_TEST_CASE(test_get_start_of_month_milliseconds)
    {
        const auto current_milliseconds = celeritas::time_helper::get_current_milliseconds();
        const auto start_of_month_milliseconds = celeritas::time_helper::get_start_of_month_milliseconds();

        BOOST_CHECK_LE(start_of_month_milliseconds, current_milliseconds);
        BOOST_CHECK_GE(current_milliseconds - start_of_month_milliseconds, 0);
        BOOST_CHECK_LT(current_milliseconds - start_of_month_milliseconds, celeritas::max_month_milliseconds);

        const auto [date, time_since_midnight] = get_time_components(start_of_month_milliseconds);
        const std::chrono::year_month_day year_month_day{ date };

        // 检查时间分量是否为午夜
        BOOST_CHECK_EQUAL(time_since_midnight.count(), 0);
        // 检查是否为月份的第一天
        BOOST_CHECK_EQUAL(static_cast<unsigned int>(year_month_day.day()), 1u);
    }

    BOOST_AUTO_TEST_CASE(test_get_start_of_month_milliseconds_with_offset)
    {
        const auto start_of_month_milliseconds = celeritas::time_helper::get_start_of_month_milliseconds();

        const auto [date, time_since_midnight] = get_time_components(start_of_month_milliseconds);
        const std::chrono::year_month_day year_month_day{ date };

        const auto start_of_month_offset_zero_milliseconds = celeritas::time_helper::get_start_of_month_milliseconds_with_offset(0);
        BOOST_CHECK_EQUAL(start_of_month_offset_zero_milliseconds, start_of_month_milliseconds);

        const auto prev_month_year_month_day = year_month_day - std::chrono::months(1);
        const auto expected_prev_month_start_tp = std::chrono::sys_days(prev_month_year_month_day);
        const auto expected_prev_month_start_milliseconds = std::chrono::time_point_cast<std::chrono::milliseconds>(expected_prev_month_start_tp).time_since_epoch().count();
        const auto current_milliseconds = celeritas::time_helper::get_current_milliseconds();

        for (auto i = 0; i < celeritas::min_month; ++i)
        {
            const auto offset = i * celeritas::day_milliseconds;
            const auto result = celeritas::time_helper::get_start_of_month_milliseconds_with_offset(offset);

            if (start_of_month_milliseconds + offset < current_milliseconds)
            {
                BOOST_CHECK_EQUAL(result, start_of_month_milliseconds + offset);
            }
            else
            {
                BOOST_CHECK_EQUAL(result, expected_prev_month_start_milliseconds + offset);
            }
        }
    }

    BOOST_AUTO_TEST_CASE(test_to_milliseconds)
    {
        const auto now_time_point = std::chrono::system_clock::now();
        const auto now_milliseconds = celeritas::time_helper::to_milliseconds(now_time_point);

        const auto expected_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now_time_point.time_since_epoch()).count();
        BOOST_CHECK_EQUAL(now_milliseconds, expected_milliseconds);

        // 使用特定时间点 (Epoch) 进行测试
        const auto specific_tp_epoch = std::chrono::system_clock::from_time_t(0);
        BOOST_CHECK_EQUAL(celeritas::time_helper::to_milliseconds(specific_tp_epoch), 0);

        // 使用特定时间点 (Epoch 后 100 秒) 进行测试
        const auto specific_tp_100s = std::chrono::system_clock::from_time_t(100);
        BOOST_CHECK_EQUAL(celeritas::time_helper::to_milliseconds(specific_tp_100s), 100 * celeritas::milliseconds);
    }

BOOST_AUTO_TEST_SUITE_END()
