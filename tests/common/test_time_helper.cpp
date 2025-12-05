#include "common/time_helper.h"

#include <boost/test/unit_test.hpp>

#include <thread>

BOOST_AUTO_TEST_SUITE(time_helper_suite)

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
        // 允许一些调度延迟
        BOOST_CHECK_LE(milliseconds2, milliseconds1 + 2000);
    }

BOOST_AUTO_TEST_SUITE_END()
