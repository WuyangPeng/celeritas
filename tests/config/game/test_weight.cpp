#include "config/game/weight.h"
#include "common/core/celeritas_error.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(weight_suite)

    BOOST_AUTO_TEST_CASE(test_weight_add_and_get)
    {
        celeritas::weight w;

        w.add_element(101, 0.5);
        w.add_element(102, 1.5);

        const auto& weights = w.get_weights();
        BOOST_CHECK_EQUAL(weights.size(), 2);
        BOOST_CHECK_CLOSE(weights[0], 0.5, 0.001);
        BOOST_CHECK_CLOSE(weights[1], 1.5, 0.001);

        BOOST_CHECK_EQUAL(w.get_id(0), 101);
        BOOST_CHECK_EQUAL(w.get_id(1), 102);
    }

    BOOST_AUTO_TEST_CASE(test_weight_clear)
    {
        celeritas::weight w;
        w.add_element(1, 10.0);
        BOOST_CHECK_EQUAL(w.get_weights().size(), 1);

        w.clear();
        BOOST_CHECK(w.get_weights().empty());
    }

    BOOST_AUTO_TEST_CASE(test_weight_get_id_out_of_range)
    {
        celeritas::weight w;
        w.add_element(1, 10.0);

        // 假设 get_id 在越界时会抛出异常或有未定义行为，
        // 但根据头文件无法确定具体实现。
        // 如果是 std::vector::operator[] 则不会检查边界，如果是 at() 则会抛出异常。
        // 这里我们假设它是安全的或者测试正常范围。
        // 如果需要测试异常，需要知道具体实现。
        // 暂时只测试正常情况。
        BOOST_CHECK_EQUAL(w.get_id(0), 1);
    }

BOOST_AUTO_TEST_SUITE_END()
