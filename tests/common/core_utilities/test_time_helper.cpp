#include "common/common_fwd.h"
#include "common/core_utilities/time_helper.h"

#include <boost/test/unit_test.hpp>

#include <chrono>
#include <thread>

BOOST_AUTO_TEST_SUITE(time_helper_suite)

    struct time_components
    {
        std::chrono::local_days date;
        std::chrono::milliseconds time_since_midnight;
    };

    [[nodiscard]] time_components get_time_components(const int64_t milliseconds)
    {
        const auto time_point = std::chrono::system_clock::time_point(std::chrono::milliseconds(milliseconds));
        const auto local_time = celeritas::time_helper::to_local_time(time_point);
        const auto date = std::chrono::floor<std::chrono::days>(local_time);
        const auto time_since_midnight_raw = local_time - date;
        const auto time_since_midnight_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(time_since_midnight_raw);

        return { date, time_since_midnight_milliseconds };
    }

    BOOST_AUTO_TEST_CASE(test_get_current_time)
    {
        const auto current_seconds = celeritas::time_helper::get_current_seconds();
        const auto current_milliseconds = celeritas::time_helper::get_current_milliseconds();

        BOOST_CHECK_GE(current_milliseconds / 1000, current_seconds);
        BOOST_CHECK_LE(current_milliseconds / 1000, current_seconds + 1);

        // 休眠一小段时间以确保时间推移
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));

        const auto later_seconds = celeritas::time_helper::get_current_seconds();
        const auto later_milliseconds = celeritas::time_helper::get_current_milliseconds();

        BOOST_CHECK_GE(later_seconds, current_seconds + 1);
        BOOST_CHECK_LE(later_seconds, current_seconds + 2);

        BOOST_CHECK_GE(later_milliseconds, current_milliseconds + 1500);
        BOOST_CHECK_LE(later_milliseconds, current_milliseconds + 2000);
    }

    BOOST_AUTO_TEST_CASE(test_get_start_of_day_milliseconds)
    {
        const auto current_milliseconds = celeritas::time_helper::get_current_milliseconds();
        const auto day_start_milliseconds = celeritas::time_helper::get_start_of_day_milliseconds();

        BOOST_CHECK_LE(day_start_milliseconds, current_milliseconds);
        BOOST_CHECK_GE(current_milliseconds - day_start_milliseconds, 0);
        BOOST_CHECK_LT(current_milliseconds - day_start_milliseconds, celeritas::day_milliseconds);

        const auto [local_date, time_since_midnight] = get_time_components(day_start_milliseconds);

        BOOST_CHECK_EQUAL(time_since_midnight.count(), 0);
    }

    BOOST_AUTO_TEST_CASE(test_get_start_of_day_milliseconds_with_offset)
    {
        const auto day_start_milliseconds = celeritas::time_helper::get_start_of_day_milliseconds();
        const auto prev_day_start_milliseconds = day_start_milliseconds - celeritas::day_milliseconds;
        const auto current_milliseconds = celeritas::time_helper::get_current_milliseconds();

        for (auto i = 0; i < celeritas::day_hour; ++i)
        {
            const auto offset = i * celeritas::hour_milliseconds;
            const auto result = celeritas::time_helper::get_start_of_day_milliseconds_with_offset(offset);

            if (day_start_milliseconds + offset < current_milliseconds)
            {
                BOOST_CHECK_EQUAL(result, day_start_milliseconds + offset);
            }
            else
            {
                BOOST_CHECK_EQUAL(result, prev_day_start_milliseconds + offset);
            }
        }
    }

    BOOST_AUTO_TEST_CASE(test_get_start_of_day_milliseconds_with_offset_and_check_time)
    {
        const auto day_start_milliseconds = celeritas::time_helper::get_start_of_day_milliseconds();
        const auto prev_day_start_milliseconds = day_start_milliseconds - celeritas::day_milliseconds;
        const auto current_milliseconds = celeritas::time_helper::get_current_milliseconds();

        for (auto i = 0; i < celeritas::day_hour; ++i)
        {
            const auto offset = i * celeritas::hour_milliseconds;
            const auto result = celeritas::time_helper::get_start_of_day_milliseconds_with_offset(current_milliseconds, offset);

            if (day_start_milliseconds + offset < current_milliseconds)
            {
                BOOST_CHECK_EQUAL(result, day_start_milliseconds + offset);
            }
            else
            {
                BOOST_CHECK_EQUAL(result, prev_day_start_milliseconds + offset);
            }
        }
    }

    BOOST_AUTO_TEST_CASE(test_get_start_of_week_milliseconds)
    {
        const auto current_milliseconds = celeritas::time_helper::get_current_milliseconds();
        const auto week_start_milliseconds = celeritas::time_helper::get_start_of_week_milliseconds();

        BOOST_CHECK_LE(week_start_milliseconds, current_milliseconds);
        BOOST_CHECK_GE(current_milliseconds - week_start_milliseconds, 0);
        BOOST_CHECK_LT(current_milliseconds - week_start_milliseconds, celeritas::week_milliseconds);

        const auto [local_date, time_since_midnight] = get_time_components(week_start_milliseconds);
        const std::chrono::weekday weekday{ local_date };

        // 检查时间分量是否为午夜
        BOOST_CHECK_EQUAL(time_since_midnight.count(), 0);
        // 检查是否为星期一
        BOOST_CHECK_EQUAL(weekday.c_encoding(), std::chrono::Monday.c_encoding());
    }

    BOOST_AUTO_TEST_CASE(test_get_start_of_week_milliseconds_with_offset)
    {
        const auto week_start_milliseconds = celeritas::time_helper::get_start_of_week_milliseconds();
        const auto week_start_offset_zero_milliseconds = celeritas::time_helper::get_start_of_week_milliseconds_with_offset(0);
        BOOST_CHECK_EQUAL(week_start_offset_zero_milliseconds, week_start_milliseconds);

        const auto current_milliseconds = celeritas::time_helper::get_current_milliseconds();
        const auto prev_week_start_milliseconds = week_start_milliseconds - celeritas::week_milliseconds;

        for (auto i = 0; i < celeritas::week; ++i)
        {
            const auto offset = i * celeritas::day_milliseconds;
            const auto result = celeritas::time_helper::get_start_of_week_milliseconds_with_offset(offset);

            if (week_start_milliseconds + offset < current_milliseconds)
            {
                BOOST_CHECK_EQUAL(result, week_start_milliseconds + offset);
            }
            else
            {
                BOOST_CHECK_EQUAL(result, prev_week_start_milliseconds + offset);
            }
        }
    }

    BOOST_AUTO_TEST_CASE(test_get_start_of_week_milliseconds_with_offset_and_check_time)
    {
        const auto week_start_milliseconds = celeritas::time_helper::get_start_of_week_milliseconds();
        const auto current_milliseconds = celeritas::time_helper::get_current_milliseconds();
        const auto prev_week_start_milliseconds = week_start_milliseconds - celeritas::week_milliseconds;

        for (auto i = 0; i < celeritas::week; ++i)
        {
            const auto offset = i * celeritas::day_milliseconds;
            const auto result = celeritas::time_helper::get_start_of_week_milliseconds_with_offset(current_milliseconds, offset);

            if (week_start_milliseconds + offset < current_milliseconds)
            {
                BOOST_CHECK_EQUAL(result, week_start_milliseconds + offset);
            }
            else
            {
                BOOST_CHECK_EQUAL(result, prev_week_start_milliseconds + offset);
            }
        }
    }

    BOOST_AUTO_TEST_CASE(test_get_start_of_month_milliseconds)
    {
        const auto current_milliseconds = celeritas::time_helper::get_current_milliseconds();
        const auto month_start_milliseconds = celeritas::time_helper::get_start_of_month_milliseconds();

        BOOST_CHECK_LE(month_start_milliseconds, current_milliseconds);
        BOOST_CHECK_GE(current_milliseconds - month_start_milliseconds, 0);
        BOOST_CHECK_LT(current_milliseconds - month_start_milliseconds, celeritas::max_month_milliseconds);

        const auto [local_date, time_since_midnight] = get_time_components(month_start_milliseconds);
        const std::chrono::year_month_day calendar_date{ local_date };

        // 检查时间分量是否为午夜
        BOOST_CHECK_EQUAL(time_since_midnight.count(), 0);
        // 检查是否为月份的第一天
        BOOST_CHECK_EQUAL(static_cast<unsigned int>(calendar_date.day()), 1u);
    }

    BOOST_AUTO_TEST_CASE(test_get_start_of_month_milliseconds_with_offset)
    {
        const auto month_start_milliseconds = celeritas::time_helper::get_start_of_month_milliseconds();

        const auto [local_date, time_since_midnight] = get_time_components(month_start_milliseconds);
        const std::chrono::year_month_day calendar_date{ local_date };

        const auto month_start_offset_zero_milliseconds = celeritas::time_helper::get_start_of_month_milliseconds_with_offset(0);
        BOOST_CHECK_EQUAL(month_start_offset_zero_milliseconds, month_start_milliseconds);

        const auto prev_month_calendar_date = calendar_date - std::chrono::months(1);
        const auto prev_month_start_time_point = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::sys_days(prev_month_calendar_date));
        const auto prev_month_start_local = std::chrono::floor<std::chrono::days>(celeritas::time_helper::to_local_time(prev_month_start_time_point));
        const auto current_month_start_sys_days = celeritas::time_helper::get_local_zone()->to_sys(std::chrono::local_days{ prev_month_start_local });

        const auto prev_month_start_milliseconds = std::chrono::time_point_cast<std::chrono::milliseconds>(current_month_start_sys_days).time_since_epoch().count();
        const auto current_milliseconds = celeritas::time_helper::get_current_milliseconds();

        for (auto i = 0; i < celeritas::min_month; ++i)
        {
            const auto offset = i * celeritas::day_milliseconds;
            const auto result = celeritas::time_helper::get_start_of_month_milliseconds_with_offset(offset);

            if (month_start_milliseconds + offset < current_milliseconds)
            {
                BOOST_CHECK_EQUAL(result, month_start_milliseconds + offset);
            }
            else
            {
                BOOST_CHECK_EQUAL(result, prev_month_start_milliseconds + offset);
            }
        }
    }

    BOOST_AUTO_TEST_CASE(test_get_start_of_month_milliseconds_with_offset_and_check_time)
    {
        const auto month_start_milliseconds = celeritas::time_helper::get_start_of_month_milliseconds();

        const auto [local_date, time_since_midnight] = get_time_components(month_start_milliseconds);
        const std::chrono::year_month_day calendar_date{ local_date };

        const auto prev_month_calendar_date = calendar_date - std::chrono::months(1);
        const auto prev_month_start_time_point = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::sys_days(prev_month_calendar_date));
        const auto prev_month_start_local = std::chrono::floor<std::chrono::days>(celeritas::time_helper::to_local_time(prev_month_start_time_point));
        const auto current_month_start_sys_days = celeritas::time_helper::get_local_zone()->to_sys(std::chrono::local_days{ prev_month_start_local });

        const auto prev_month_start_milliseconds = std::chrono::time_point_cast<std::chrono::milliseconds>(current_month_start_sys_days).time_since_epoch().count();
        const auto current_milliseconds = celeritas::time_helper::get_current_milliseconds();

        for (auto i = 0; i < celeritas::min_month; ++i)
        {
            const auto offset = i * celeritas::day_milliseconds;
            const auto result = celeritas::time_helper::get_start_of_month_milliseconds_with_offset(current_milliseconds, offset);

            if (month_start_milliseconds + offset < current_milliseconds)
            {
                BOOST_CHECK_EQUAL(result, month_start_milliseconds + offset);
            }
            else
            {
                BOOST_CHECK_EQUAL(result, prev_month_start_milliseconds + offset);
            }
        }
    }

    BOOST_AUTO_TEST_CASE(test_get_next_day_start_milliseconds)
    {
        const auto current_milliseconds = celeritas::time_helper::get_current_milliseconds();
        const auto next_day_start_milliseconds = celeritas::time_helper::get_next_day_start_milliseconds();
        const auto day_start_milliseconds = celeritas::time_helper::get_start_of_day_milliseconds();

        BOOST_CHECK_GT(next_day_start_milliseconds, current_milliseconds);
        BOOST_CHECK_EQUAL(next_day_start_milliseconds, day_start_milliseconds + celeritas::day_milliseconds);

        const auto [local_date, time_since_midnight] = get_time_components(next_day_start_milliseconds);
        BOOST_CHECK_EQUAL(time_since_midnight.count(), 0);
    }

    BOOST_AUTO_TEST_CASE(test_get_next_day_start_milliseconds_with_offset)
    {
        const auto current_milliseconds = celeritas::time_helper::get_current_milliseconds();
        const auto day_start_milliseconds = celeritas::time_helper::get_start_of_day_milliseconds();
        const auto next_day_start_milliseconds = day_start_milliseconds + celeritas::day_milliseconds;

        for (auto i = 0; i < celeritas::day_hour; ++i)
        {
            const auto offset = i * celeritas::hour_milliseconds;
            const auto result = celeritas::time_helper::get_next_day_start_milliseconds_with_offset(offset);

            if (current_milliseconds < day_start_milliseconds + offset)
            {
                BOOST_CHECK_EQUAL(result, day_start_milliseconds + offset);
            }
            else
            {
                BOOST_CHECK_EQUAL(result, next_day_start_milliseconds + offset);
            }
        }
    }

    BOOST_AUTO_TEST_CASE(test_get_next_day_start_milliseconds_with_check_time)
    {
        const auto current_milliseconds = celeritas::time_helper::get_current_milliseconds();
        const auto day_start_milliseconds = celeritas::time_helper::get_start_of_day_milliseconds();
        const auto next_day_start_milliseconds = day_start_milliseconds + celeritas::day_milliseconds;

        for (auto i = 0; i < celeritas::day_hour; ++i)
        {
            const auto offset = i * celeritas::hour_milliseconds;
            const auto result = celeritas::time_helper::get_next_day_start_milliseconds_with_offset(current_milliseconds, offset);

            if (current_milliseconds < day_start_milliseconds + offset)
            {
                BOOST_CHECK_EQUAL(result, day_start_milliseconds + offset);
            }
            else
            {
                BOOST_CHECK_EQUAL(result, next_day_start_milliseconds + offset);
            }
        }
    }

    BOOST_AUTO_TEST_CASE(test_get_next_week_start_milliseconds)
    {
        const auto current_milliseconds = celeritas::time_helper::get_current_milliseconds();
        const auto next_week_start_milliseconds = celeritas::time_helper::get_next_week_start_milliseconds();
        const auto week_start_milliseconds = celeritas::time_helper::get_start_of_week_milliseconds();

        BOOST_CHECK_GT(next_week_start_milliseconds, current_milliseconds);
        BOOST_CHECK_EQUAL(next_week_start_milliseconds, week_start_milliseconds + celeritas::week_milliseconds);

        const auto [local_date, time_since_midnight] = get_time_components(next_week_start_milliseconds);
        const std::chrono::weekday weekday{ local_date };

        BOOST_CHECK_EQUAL(time_since_midnight.count(), 0);
        BOOST_CHECK_EQUAL(weekday.c_encoding(), std::chrono::Monday.c_encoding());
    }

    BOOST_AUTO_TEST_CASE(test_get_next_week_start_milliseconds_with_offset)
    {
        const auto current_milliseconds = celeritas::time_helper::get_current_milliseconds();
        const auto week_start_milliseconds = celeritas::time_helper::get_start_of_week_milliseconds();
        const auto next_week_start_milliseconds = week_start_milliseconds + celeritas::week_milliseconds;

        for (auto i = 0; i < celeritas::week; ++i)
        {
            const auto offset = i * celeritas::day_milliseconds;
            const auto result = celeritas::time_helper::get_next_week_start_milliseconds_with_offset(offset);

            if (current_milliseconds < week_start_milliseconds + offset)
            {
                BOOST_CHECK_EQUAL(result, week_start_milliseconds + offset);
            }
            else
            {
                BOOST_CHECK_EQUAL(result, next_week_start_milliseconds + offset);
            }
        }
    }

    BOOST_AUTO_TEST_CASE(test_get_next_week_start_milliseconds_with_check_time)
    {
        const auto current_milliseconds = celeritas::time_helper::get_current_milliseconds();
        const auto week_start_milliseconds = celeritas::time_helper::get_start_of_week_milliseconds();
        const auto next_week_start_milliseconds = week_start_milliseconds + celeritas::week_milliseconds;

        for (auto i = 0; i < celeritas::week; ++i)
        {
            const auto offset = i * celeritas::day_milliseconds;
            const auto result = celeritas::time_helper::get_next_week_start_milliseconds_with_offset(current_milliseconds, offset);

            if (current_milliseconds < week_start_milliseconds + offset)
            {
                BOOST_CHECK_EQUAL(result, week_start_milliseconds + offset);
            }
            else
            {
                BOOST_CHECK_EQUAL(result, next_week_start_milliseconds + offset);
            }
        }
    }

    BOOST_AUTO_TEST_CASE(test_get_next_month_start_milliseconds)
    {
        const auto current_milliseconds = celeritas::time_helper::get_current_milliseconds();
        const auto next_month_start_milliseconds = celeritas::time_helper::get_next_month_start_milliseconds();
        const auto month_start_milliseconds = celeritas::time_helper::get_start_of_month_milliseconds();

        const auto [start_local_date, start_time] = get_time_components(month_start_milliseconds);
        const std::chrono::year_month_day calendar_date{ start_local_date };
        const auto next_month_calendar_date = calendar_date + std::chrono::months(1);
        const auto next_month_start_time_point = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::sys_days(next_month_calendar_date));
        const auto next_month_start_local = std::chrono::floor<std::chrono::days>(celeritas::time_helper::to_local_time(next_month_start_time_point));
        const auto next_month_start_sys_days = celeritas::time_helper::get_local_zone()->to_sys(std::chrono::local_days{ next_month_start_local });
        const auto expected_next_month_start = celeritas::time_helper::to_milliseconds(next_month_start_sys_days);

        BOOST_CHECK_GT(next_month_start_milliseconds, current_milliseconds);
        BOOST_CHECK_EQUAL(next_month_start_milliseconds, expected_next_month_start);

        const auto [local_date, time_since_midnight] = get_time_components(next_month_start_milliseconds);
        const std::chrono::year_month_day end_calendar_date{ local_date };

        BOOST_CHECK_EQUAL(time_since_midnight.count(), 0);
        BOOST_CHECK_EQUAL(static_cast<unsigned int>(end_calendar_date.day()), 1u);
    }

    BOOST_AUTO_TEST_CASE(test_get_next_month_start_milliseconds_with_offset)
    {
        const auto current_milliseconds = celeritas::time_helper::get_current_milliseconds();
        const auto month_start_milliseconds = celeritas::time_helper::get_start_of_month_milliseconds();

        const auto [start_local_date, start_time] = get_time_components(month_start_milliseconds);
        const std::chrono::year_month_day calendar_date{ start_local_date };
        const auto next_month_calendar_date = calendar_date + std::chrono::months(1);
        const auto next_month_start_time_point = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::sys_days(next_month_calendar_date));
        const auto next_month_start_local = std::chrono::floor<std::chrono::days>(celeritas::time_helper::to_local_time(next_month_start_time_point));
        const auto next_month_start_sys_days = celeritas::time_helper::get_local_zone()->to_sys(std::chrono::local_days{ next_month_start_local });
        const auto next_month_start_milliseconds = celeritas::time_helper::to_milliseconds(next_month_start_sys_days);

        for (auto i = 0; i < celeritas::min_month; ++i)
        {
            const auto offset = i * celeritas::day_milliseconds;
            const auto result = celeritas::time_helper::get_next_month_start_milliseconds_with_offset(offset);

            if (current_milliseconds < month_start_milliseconds + offset)
            {
                BOOST_CHECK_EQUAL(result, month_start_milliseconds + offset);
            }
            else
            {
                BOOST_CHECK_EQUAL(result, next_month_start_milliseconds + offset);
            }
        }
    }

    BOOST_AUTO_TEST_CASE(test_get_next_month_start_milliseconds_with_check_time)
    {
        const auto current_milliseconds = celeritas::time_helper::get_current_milliseconds();
        const auto month_start_milliseconds = celeritas::time_helper::get_start_of_month_milliseconds();

        const auto [start_local_date, start_time] = get_time_components(month_start_milliseconds);
        const std::chrono::year_month_day calendar_date{ start_local_date };
        const auto next_month_calendar_date = calendar_date + std::chrono::months(1);
        const auto next_month_start_time_point = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::sys_days(next_month_calendar_date));
        const auto next_month_start_local = std::chrono::floor<std::chrono::days>(celeritas::time_helper::to_local_time(next_month_start_time_point));
        const auto next_month_start_sys_days = celeritas::time_helper::get_local_zone()->to_sys(std::chrono::local_days{ next_month_start_local });
        const auto next_month_start_milliseconds = celeritas::time_helper::to_milliseconds(next_month_start_sys_days);

        for (auto i = 0; i < celeritas::min_month; ++i)
        {
            const auto offset = i * celeritas::day_milliseconds;
            const auto result = celeritas::time_helper::get_next_month_start_milliseconds_with_offset(current_milliseconds, offset);

            if (current_milliseconds < month_start_milliseconds + offset)
            {
                BOOST_CHECK_EQUAL(result, month_start_milliseconds + offset);
            }
            else
            {
                BOOST_CHECK_EQUAL(result, next_month_start_milliseconds + offset);
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
