#include "celeritas_error.h"
#include "common_fwd.h"
#include "snowflake_generator.h"

#include <chrono>
#include <mutex>

celeritas::snowflake_generator& celeritas::snowflake_generator::get_instance()
{
    static snowflake_generator generator{};

    return generator;
}

int64_t celeritas::snowflake_generator::generate(const int datacenter_id, const int worker_id)
{
    auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    if (timestamp < last_timestamp_)
    {
        if (const auto offset = last_timestamp_ - timestamp;
            offset < max_clock_skew_ms)
        {
            timestamp = til_next_millis(last_timestamp_);
        }
        else
        {
            throw celeritas_error("clock moved backwards. Refusing to generate id for " + std::to_string(offset) + " milliseconds.");
        }
    }

    if (last_timestamp_ == timestamp)
    {
        sequence_ = (sequence_ + 1) & sequence_mask;
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

    return ((timestamp - epoch) << timestamp_left_shift) |
           (datacenter_id << datacenter_id_shift) |
           (worker_id << worker_id_shift) |
           sequence_;
}

celeritas::snowflake_generator::snowflake_generator() noexcept
    : id_mutex_{}, last_timestamp_{ -1 }, sequence_{ 0 }
{
}

int64_t celeritas::snowflake_generator::til_next_millis(const int64_t last_timestamp)
{
    auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    while (timestamp <= last_timestamp)
    {
        timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }
    return timestamp;
}
