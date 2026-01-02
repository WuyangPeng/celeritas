#include "common/common_constant.h"
#include "common/core/snowflake_generator.h"
#include "fixture/sequence_increment_fixture.h"
#include "fixture/uniqueness_fixture.h"

#include <boost/test/unit_test.hpp>

#include <chrono>
#include <mutex>
#include <thread>
#include <vector>

namespace
{
    constexpr auto expected_datacenter_id = 5;
    constexpr auto expected_worker_id = 10;

    // 检查ID的结构（数据中心ID和工作节点ID）
    void check_id_structure(const int64_t id)
    {
        const auto extracted_datacenter_id = id >> celeritas::datacenter_id_shift & celeritas::max_datacenter_id;
        const auto extracted_worker_id = id >> celeritas::worker_id_shift & celeritas::max_worker_id;
        const auto sequence = id & celeritas::sequence_mask;

        BOOST_CHECK_EQUAL(extracted_datacenter_id, expected_datacenter_id);
        BOOST_CHECK_EQUAL(extracted_worker_id, expected_worker_id);
        BOOST_CHECK_GE(sequence, 0);
    }

    // 检查ID的时间戳是否合理
    void check_id_timestamp(const int64_t id)
    {
        const auto timestamp = id >> celeritas::timestamp_left_shift;
        const auto current_timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

        // 5秒的容差
        constexpr auto tolerance = 5000;
        BOOST_CHECK_GE(timestamp + celeritas::epoch, current_timestamp - tolerance);
        BOOST_CHECK_LE(timestamp + celeritas::epoch, current_timestamp + tolerance);
    }
}

BOOST_AUTO_TEST_SUITE(snowflake_generator_suite)

    // 测试生成的ID的唯一性
    BOOST_FIXTURE_TEST_CASE(test_uniqueness, celeritas::uniqueness_fixture)
    {
        init();

        BOOST_CHECK_EQUAL(get_generated_ids_size(), num_threads * ids_per_thread);
    }

    // 测试ID的组成部分
    BOOST_AUTO_TEST_CASE(test_id_components)
    {
        const auto id = celeritas::snowflake_generator::get_instance().generate(expected_datacenter_id, expected_worker_id);

        check_id_structure(id);
        check_id_timestamp(id);
    }

    // 测试同一毫秒内序列的递增
    BOOST_FIXTURE_TEST_CASE(test_sequence_increment, celeritas::sequence_increment_fixture)
    {
        init();

        check_sequence_monotonicity();
    }

BOOST_AUTO_TEST_SUITE_END()