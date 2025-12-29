#include "core_utilities/celeritas_error.h"
#include "common_fwd.h"
#include "snowflake_generator.h"
#include "time_helper.h"

#include <mutex>

celeritas::snowflake_generator& celeritas::snowflake_generator::get_instance()
{
    static snowflake_generator generator{};

    return generator;
}

int64_t celeritas::snowflake_generator::generate(const int datacenter_id, const int worker_id)
{
    std::lock_guard lock{ id_mutex_ };

    // 时间获得比须在获得锁之后，不然可能是锁住之前的时间
    auto timestamp = time_helper::get_current_milliseconds();

    if (timestamp < last_timestamp_)
    {
        timestamp = handle_clock_backwards(timestamp);
    }

    update_timestamp_and_sequence(timestamp);

    return (last_timestamp_ - epoch) << timestamp_left_shift |
           static_cast<int64_t>(datacenter_id) << datacenter_id_shift |
           static_cast<int64_t>(worker_id) << worker_id_shift |
           sequence_;
}

celeritas::snowflake_generator::snowflake_generator() noexcept
    : id_mutex_{}, last_timestamp_{ -1 }, sequence_{ 0 }
{
}

int64_t celeritas::snowflake_generator::til_next_millis(const int64_t last_timestamp)
{
    auto timestamp = time_helper::get_current_milliseconds();

    while (timestamp <= last_timestamp)
    {
        timestamp = time_helper::get_current_milliseconds();
    }

    return timestamp;
}

int64_t celeritas::snowflake_generator::handle_clock_backwards(const int64_t timestamp) const
{
    const auto offset = last_timestamp_ - timestamp;
    if (offset < max_clock_skew_ms)
    {
        return til_next_millis(last_timestamp_);
    }

    throw celeritas_error{ "clock moved backwards. Refusing to generate id for {} milliseconds.", offset };
}

void celeritas::snowflake_generator::update_timestamp_and_sequence(int64_t timestamp)
{
    if (last_timestamp_ == timestamp)
    {
        sequence_ = sequence_ + 1 & sequence_mask;
        if (sequence_ == 0)
        {
            timestamp = til_next_millis(last_timestamp_);
        }
    }
    else
    {
        sequence_ = 0;
    }

    last_timestamp_ = timestamp;
}
