#include "sequence_increment_fixture.h"
#include "common/common_constant.h"
#include "common/core/snowflake_generator.h"

#include <boost/test/unit_test.hpp>

void celeritas::sequence_increment_fixture::init()
{
    ids_.reserve(num_ids);

    for (auto i = 0; i < num_ids; ++i)
    {
        constexpr auto datacenter_id = 1;
        constexpr auto worker_id = 1;
        ids_.emplace_back(snowflake_generator::get_instance().generate(datacenter_id, worker_id));
    }
}

void celeritas::sequence_increment_fixture::check_sequence_monotonicity()
{
    for (auto i = 1; i < ids_.size(); ++i)
    {
        do_check_sequence_monotonicity(i);
    }
}

void celeritas::sequence_increment_fixture::do_check_sequence_monotonicity(const int index)
{
    const auto prev_id = ids_.at(index - 1);
    const auto current_id = ids_.at(index);

    const auto prev_timestamp = prev_id >> timestamp_left_shift;

    if (const auto current_timestamp = current_id >> timestamp_left_shift;
        prev_timestamp == current_timestamp)
    {
        const auto prev_sequence = prev_id & sequence_mask;
        const auto current_sequence = current_id & sequence_mask;
        BOOST_CHECK_EQUAL(current_sequence, prev_sequence + 1);
    }
}