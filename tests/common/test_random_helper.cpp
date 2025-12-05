#include "common/celeritas_error.h"
#include "common/random_helper.h"

#include <boost/test/unit_test.hpp>

#include <numeric>
#include <vector>

BOOST_AUTO_TEST_SUITE(random_helper_suite)

    // 测试 get_random_int(int end)
    BOOST_AUTO_TEST_CASE(test_get_random_int_end_range)
    {
        for (auto i = 0; i < 1000; ++i)
        {
            constexpr auto end = 100;
            const auto random_val = celeritas::random_helper::get_random_int(end);
            BOOST_CHECK_GE(random_val, 0);
            BOOST_CHECK_LT(random_val, end);
        }
    }

    BOOST_AUTO_TEST_CASE(test_get_random_int_end_zero)
    {
        const auto random_value = celeritas::random_helper::get_random_int(1);
        BOOST_CHECK_EQUAL(random_value, 0);
    }

    // 测试 get_random_int(int begin, int end)
    BOOST_AUTO_TEST_CASE(test_get_random_int_begin_end_range)
    {
        for (auto i = 0; i < 1000; ++i)
        {
            constexpr auto begin = -50;
            constexpr auto end = 50;

            const auto random_val = celeritas::random_helper::get_random_int(begin, end);
            BOOST_CHECK_GE(random_val, begin);
            BOOST_CHECK_LT(random_val, end);
        }
    }

    BOOST_AUTO_TEST_CASE(test_get_random_int_begin_end_equal)
    {
        constexpr auto value = 10;
        const auto random_value = celeritas::random_helper::get_random_int(value, value + 1);
        BOOST_CHECK_EQUAL(random_value, value);
    }

    // 新增测试：get_random_int 在 begin > end 时抛出异常
    BOOST_AUTO_TEST_CASE(test_get_random_int_begin_greater_than_end_throws)
    {
        constexpr auto begin = 10;
        constexpr auto end = 5;
        BOOST_CHECK_THROW([] { std::ignore = celeritas::random_helper::get_random_int(begin, end); }(), celeritas::celeritas_error);
    }

    // 测试 get_random_float()
    BOOST_AUTO_TEST_CASE(test_get_random_float_range)
    {
        for (auto i = 0; i < 1000; ++i)
        {
            const auto random_value = celeritas::random_helper::get_random_float();
            BOOST_CHECK_GE(random_value, 0.0f);
            BOOST_CHECK_LT(random_value, 1.0f);
        }
    }

    // 测试 get_random_float(float begin, float end)
    BOOST_AUTO_TEST_CASE(test_get_random_float_begin_end_range)
    {
        for (auto i = 0; i < 1000; ++i)
        {
            constexpr auto begin = -10.0f;
            constexpr auto end = 10.0f;

            const auto random_value = celeritas::random_helper::get_random_float(begin, end);
            BOOST_CHECK_GE(random_value, begin);
            BOOST_CHECK_LT(random_value, end);
        }
    }

    // 新增测试：get_random_float 在 begin > end 时抛出异常
    BOOST_AUTO_TEST_CASE(test_get_random_float_begin_greater_than_end_throws)
    {
        constexpr auto begin = 10.0f;
        constexpr auto end = 5.0f;
        BOOST_CHECK_THROW([] { std::ignore = celeritas::random_helper::get_random_float(begin, end); }(), celeritas::celeritas_error);
    }

    // 测试 get_random_double()
    BOOST_AUTO_TEST_CASE(test_get_random_double_range)
    {
        for (auto i = 0; i < 1000; ++i)
        {
            const auto random_value = celeritas::random_helper::get_random_double();
            BOOST_CHECK_GE(random_value, 0.0);
            BOOST_CHECK_LT(random_value, 1.0);
        }
    }

    // 测试 get_random_double(double begin, double end)
    BOOST_AUTO_TEST_CASE(test_get_random_double_begin_end_range)
    {
        for (auto i = 0; i < 1000; ++i)
        {
            constexpr auto begin = -100.0;
            constexpr auto end = 100.0;

            const auto random_value = celeritas::random_helper::get_random_double(begin, end);
            BOOST_CHECK_GE(random_value, begin);
            BOOST_CHECK_LT(random_value, end);
        }
    }

    // 新增测试：get_random_double 在 begin > end 时抛出异常
    BOOST_AUTO_TEST_CASE(test_get_random_double_begin_greater_than_end_throws)
    {
        constexpr auto begin = 100.0;
        constexpr auto end = 50.0;
        BOOST_CHECK_THROW([] { std::ignore = celeritas::random_helper::get_random_double(begin, end); }(), celeritas::celeritas_error);
    }

    // 测试 get_random_bool(double p)
    BOOST_AUTO_TEST_CASE(test_get_random_bool_probability)
    {
        constexpr auto num_iterations = 10000;
        constexpr auto p_true = 0.7;
        auto true_count = 0;

        for (auto i = 0; i < num_iterations; ++i)
        {
            if (celeritas::random_helper::get_random_bool(p_true))
            {
                true_count++;
            }
        }
        // 检查真值的比例是否大致接近 p_true
        // 使用 5% 的容差进行基本检查
        BOOST_CHECK_CLOSE(static_cast<double>(true_count) / num_iterations, p_true, 5.0);
    }

    BOOST_AUTO_TEST_CASE(test_get_random_bool_always_true)
    {
        for (auto i = 0; i < 100; ++i)
        {
            BOOST_CHECK(celeritas::random_helper::get_random_bool(1.0));
        }
    }

    BOOST_AUTO_TEST_CASE(test_get_random_bool_always_false)
    {
        for (auto i = 0; i < 100; ++i)
        {
            BOOST_CHECK(!celeritas::random_helper::get_random_bool(0.0));
        }
    }

    // 测试 get_random_normal(double mean, double stddev)
    BOOST_AUTO_TEST_CASE(test_get_random_normal_basic)
    {
        constexpr auto mean_value = 10.0;
        constexpr auto stddev_value = 2.0;
        constexpr auto num_iterations = 1000;
        std::vector<double> samples{};
        samples.reserve(num_iterations);

        for (auto i = 0; i < num_iterations; ++i)
        {
            samples.emplace_back(celeritas::random_helper::get_random_normal(mean_value, stddev_value));
        }

        // 基本检查：平均值应接近均值
        const auto sum = std::accumulate(samples.begin(), samples.end(), 0.0);
        const auto average = sum / num_iterations;
        BOOST_CHECK_CLOSE(average, mean_value, 10.0); // 允许平均值有 10% 的容差

        // 基本检查：值通常应在均值的几个标准差范围内
        for (auto value : samples)
        {
            BOOST_CHECK_GE(value, mean_value - 4 * stddev_value); // 大致在 4 个标准差范围内
            BOOST_CHECK_LE(value, mean_value + 4 * stddev_value);
        }
    }

BOOST_AUTO_TEST_SUITE_END()
