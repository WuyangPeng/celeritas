#include "common/common_fwd.h"
#include "common/core_utilities/snowflake_generator.h"

#include <boost/test/unit_test.hpp>

#include <chrono>
#include <mutex>
#include <thread>
#include <unordered_set>
#include <vector>

BOOST_AUTO_TEST_SUITE(snowflake_generator_suite)

    // 测试生成的ID的唯一性
    BOOST_AUTO_TEST_CASE(test_uniqueness)
    {
        constexpr auto num_threads = 10;
        constexpr auto ids_per_thread = 1000;
        std::vector<std::thread> threads{};
        std::unordered_set<int64_t> generated_ids{};
        std::mutex set_mutex{};

        auto generate_ids = [&](const auto datacenter_id, const auto worker_id) {
            for (auto i = 0; i < ids_per_thread; ++i)
            {
                const auto id = celeritas::snowflake_generator::get_instance().generate(datacenter_id, worker_id);

                std::lock_guard lock{ set_mutex };
                BOOST_CHECK(!generated_ids.contains(id));
                generated_ids.emplace(id);
            }
        };

        for (auto i = 0; i < num_threads; ++i)
        {
            threads.emplace_back(generate_ids, i % celeritas::max_datacenter_id, i % celeritas::max_worker_id);
        }

        for (auto& thread : threads)
        {
            thread.join();
        }

        BOOST_CHECK_EQUAL(generated_ids.size(), num_threads * ids_per_thread);
    }

    // 测试ID的组成部分
    BOOST_AUTO_TEST_CASE(test_id_components)
    {
        constexpr auto datacenter_id = 5;
        constexpr auto worker_id = 10;
        const auto id = celeritas::snowflake_generator::get_instance().generate(datacenter_id, worker_id);

        // 从ID中提取各个部分
        const auto timestamp = id >> celeritas::timestamp_left_shift;
        const auto extracted_datacenter_id = id >> celeritas::datacenter_id_shift & celeritas::max_datacenter_id;
        const auto extracted_worker_id = id >> celeritas::worker_id_shift & celeritas::max_worker_id;
        const auto sequence = id & celeritas::sequence_mask;

        BOOST_CHECK_EQUAL(extracted_datacenter_id, datacenter_id);
        BOOST_CHECK_EQUAL(extracted_worker_id, worker_id);
        BOOST_CHECK_GE(sequence, 0);

        // 检查时间戳是否在合理范围内 (允许几秒的误差)
        const auto current_timestamp =
            std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

        // 5秒的容差
        constexpr auto tolerance = 5000;
        BOOST_CHECK_GE(timestamp + celeritas::epoch, current_timestamp - tolerance);
        BOOST_CHECK_LE(timestamp + celeritas::epoch, current_timestamp + tolerance);
    }

    // 测试同一毫秒内序列的递增
    BOOST_AUTO_TEST_CASE(test_sequence_increment)
    {
        constexpr auto num_ids = 10;
        std::vector<int64_t> ids{};
        ids.reserve(num_ids);

        for (auto i = 0; i < num_ids; ++i)
        {
            constexpr auto datacenter_id = 1;
            constexpr auto worker_id = 1;
            ids.emplace_back(celeritas::snowflake_generator::get_instance().generate(datacenter_id, worker_id));
        }

        for (auto i = 1u; i < ids.size(); ++i)
        {
            const auto prev_id = ids[i - 1];
            const auto current_id = ids[i];

            const auto prev_timestamp = prev_id >> celeritas::timestamp_left_shift;

            if (const auto current_timestamp = current_id >> celeritas::timestamp_left_shift;
                prev_timestamp == current_timestamp)
            {
                const auto prev_sequence = prev_id & celeritas::sequence_mask;
                const auto current_sequence = current_id & celeritas::sequence_mask;
                BOOST_CHECK_EQUAL(current_sequence, prev_sequence + 1);
            }
        }
    }

BOOST_AUTO_TEST_SUITE_END()
