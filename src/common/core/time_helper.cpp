#include "time_helper.h"
#include "common/logging/logger.h"

int64_t celeritas::time_helper::get_current_milliseconds()
{
    return to_milliseconds(std::chrono::system_clock::now());
}

int64_t celeritas::time_helper::get_current_seconds()
{
    const auto now = std::chrono::system_clock::now();

    return std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
}

int64_t celeritas::time_helper::get_start_of_day_milliseconds()
{
    return get_start_of_day_milliseconds_with_offset(0);
}

int64_t celeritas::time_helper::get_start_of_day_milliseconds_with_offset(const int64_t milliseconds_offset)
{
    const auto now = std::chrono::system_clock::now();

    return get_start_of_day_milliseconds_with_offset(now, milliseconds_offset);
}

int64_t celeritas::time_helper::get_start_of_day_milliseconds_with_offset(const int64_t check_time, const int64_t milliseconds_offset)
{
    const auto check_time_point = std::chrono::system_clock::time_point{ std::chrono::milliseconds{ check_time } };

    return get_start_of_day_milliseconds_with_offset(check_time_point, milliseconds_offset);
}

int64_t celeritas::time_helper::get_start_of_week_milliseconds()
{
    return get_start_of_week_milliseconds_with_offset(0);
}

int64_t celeritas::time_helper::get_start_of_week_milliseconds_with_offset(const int64_t milliseconds_offset)
{
    const auto now = std::chrono::system_clock::now();

    return get_start_of_week_milliseconds_with_offset(now, milliseconds_offset);
}

int64_t celeritas::time_helper::get_start_of_week_milliseconds_with_offset(const int64_t check_time, const int64_t milliseconds_offset)
{
    const auto check_time_point = std::chrono::system_clock::time_point{ std::chrono::milliseconds{ check_time } };

    return get_start_of_week_milliseconds_with_offset(check_time_point, milliseconds_offset);
}

int64_t celeritas::time_helper::get_start_of_month_milliseconds()
{
    return get_start_of_month_milliseconds_with_offset(0);
}

int64_t celeritas::time_helper::get_start_of_month_milliseconds_with_offset(const int64_t milliseconds_offset)
{
    const auto now = std::chrono::system_clock::now();

    return get_start_of_month_milliseconds_with_offset(now, milliseconds_offset);
}

int64_t celeritas::time_helper::get_start_of_month_milliseconds_with_offset(const int64_t check_time, const int64_t milliseconds_offset)
{
    const auto check_time_point = std::chrono::system_clock::time_point{ std::chrono::milliseconds{ check_time } };

    return get_start_of_month_milliseconds_with_offset(check_time_point, milliseconds_offset);
}

int64_t celeritas::time_helper::get_next_day_start_milliseconds()
{
    return get_next_day_start_milliseconds_with_offset(0);
}

int64_t celeritas::time_helper::get_next_day_start_milliseconds_with_offset(const int64_t milliseconds_offset)
{
    const auto now = std::chrono::system_clock::now();

    return get_next_day_start_milliseconds_with_offset(now, milliseconds_offset);
}

int64_t celeritas::time_helper::get_next_day_start_milliseconds_with_offset(const int64_t check_time, const int64_t milliseconds_offset)
{
    const auto check_time_point = std::chrono::system_clock::time_point{ std::chrono::milliseconds{ check_time } };

    return get_next_day_start_milliseconds_with_offset(check_time_point, milliseconds_offset);
}

int64_t celeritas::time_helper::get_next_week_start_milliseconds()
{
    return get_next_week_start_milliseconds_with_offset(0);
}

int64_t celeritas::time_helper::get_next_week_start_milliseconds_with_offset(const int64_t milliseconds_offset)
{
    const auto now = std::chrono::system_clock::now();

    return get_next_week_start_milliseconds_with_offset(now, milliseconds_offset);
}

int64_t celeritas::time_helper::get_next_week_start_milliseconds_with_offset(const int64_t check_time, const int64_t milliseconds_offset)
{
    const auto check_time_point = std::chrono::system_clock::time_point{ std::chrono::milliseconds{ check_time } };

    return get_next_week_start_milliseconds_with_offset(check_time_point, milliseconds_offset);
}

int64_t celeritas::time_helper::get_next_month_start_milliseconds()
{
    return get_next_month_start_milliseconds_with_offset(0);
}

int64_t celeritas::time_helper::get_next_month_start_milliseconds_with_offset(const int64_t milliseconds_offset)
{
    const auto now = std::chrono::system_clock::now();

    return get_next_month_start_milliseconds_with_offset(now, milliseconds_offset);
}

int64_t celeritas::time_helper::get_next_month_start_milliseconds_with_offset(const int64_t check_time, const int64_t milliseconds_offset)
{
    const auto check_time_point = std::chrono::system_clock::time_point{ std::chrono::milliseconds{ check_time } };

    return get_next_month_start_milliseconds_with_offset(check_time_point, milliseconds_offset);
}

int64_t celeritas::time_helper::to_milliseconds(const time_point_type& time_point)
{
    return std::chrono::time_point_cast<std::chrono::milliseconds>(time_point).time_since_epoch().count();
}

celeritas::time_helper::local_time_type celeritas::time_helper::to_local_time(const time_point_type& time_point)
{
    const std::chrono::zoned_time local_time{ get_local_zone(), time_point };

    return local_time.get_local_time();
}

celeritas::time_helper::const_time_zone_ptr_type celeritas::time_helper::get_local_zone()
{
    static const auto cached_zone = do_get_local_zone();

    return cached_zone;
}

celeritas::time_helper::const_time_zone_ptr_type celeritas::time_helper::do_get_local_zone()
{
    const auto* current_zone = std::chrono::get_tzdb().current_zone();

    if (current_zone->name() == "Etc/UTC")
    {
        current_zone = get_default_local_zone(current_zone);
    }
    else
    {
        LOG_CHANNEL(celeritas::common_channel, info) << "当前时区:" << current_zone->name();
    }

    return current_zone;
}

celeritas::time_helper::const_time_zone_ptr_type celeritas::time_helper::get_default_local_zone(const_time_zone_ptr_type current_zone)
{
    try
    {
        current_zone = std::chrono::get_tzdb().locate_zone("Asia/Shanghai");
        LOG_CHANNEL(celeritas::common_channel, warning) << "读不到系统本地时区，使用Asia/Shanghai。";
    }
    catch (const std::runtime_error& error)
    {
        LOG_CHANNEL(celeritas::common_channel, error) << "读不到Asia/Shanghai，使用Etc/UTC:" << error.what();
    }

    return current_zone;
}

celeritas::time_helper::local_days_type celeritas::time_helper::get_local_day(const time_point_type& time_point)
{
    return std::chrono::floor<std::chrono::days>(to_local_time(time_point));
}

celeritas::time_helper::time_point_type celeritas::time_helper::get_start_of_day(const time_point_type& time_point)
{
    const auto local_day = get_local_day(time_point);

    return get_local_zone()->to_sys(local_day);
}

celeritas::time_helper::time_point_type celeritas::time_helper::get_start_of_week(const time_point_type& time_point)
{
    const auto local_day = get_local_day(time_point);

    const std::chrono::weekday weekday{ local_day };
    const std::chrono::days days_diff{ (weekday.c_encoding() + 6) % 7 };
    const auto week_start_local = local_day - days_diff;

    return get_local_zone()->to_sys(week_start_local);
}

celeritas::time_helper::time_point_type celeritas::time_helper::get_start_of_month(const time_point_type& time_point, const int months_offset)
{
    const auto local_day = get_local_day(time_point);

    const std::chrono::year_month_day year_month_day{ local_day };
    const std::chrono::year_month_day month_start{ year_month_day.year(), year_month_day.month(), std::chrono::day{ 1 } };

    auto target_month_start = month_start;
    if (months_offset != 0)
    {
        target_month_start = month_start + std::chrono::months(months_offset);
    }

    return get_local_zone()->to_sys(std::chrono::local_days{ target_month_start });
}

int64_t celeritas::time_helper::get_start_of_day_milliseconds_with_offset(const time_point_type& check_time, const int64_t milliseconds_offset)
{
    // 获取当天的零点时间
    const auto day_start = get_start_of_day(check_time);

    if (milliseconds_offset == 0)
    {
        return to_milliseconds(day_start);
    }

    // 计算今天日历上的逻辑刷新点（零点 + 毫秒偏移）
    const auto target_time = day_start + std::chrono::milliseconds(milliseconds_offset);

    // 默认目标时间为今天的刷新点
    auto cycle_start = target_time;

    // 检查当前时间是否已经过了今天的刷新点
    if (check_time < target_time)
    {
        // 如果还没过，说明当前仍属于前一天的“逻辑日”
        // 因此，正确的周期刷新点是昨天的这个时间
        cycle_start = target_time - std::chrono::days(1);
    }

    // 将最终计算出的正确周期刷新点转换为毫秒时间戳
    return to_milliseconds(cycle_start);
}

int64_t celeritas::time_helper::get_start_of_week_milliseconds_with_offset(const time_point_type& check_time, const int64_t milliseconds_offset)
{
    // 获取本周一的零点时间
    const auto week_start = get_start_of_week(check_time);

    if (milliseconds_offset == 0)
    {
        // 将本周一的零点时间点转换为毫秒时间戳并返回
        return to_milliseconds(week_start);
    }

    // 计算出本周的逻辑刷新点（周一零点 + 毫秒偏移）
    const auto target_time = week_start + std::chrono::milliseconds(milliseconds_offset);

    // 默认目标时间为本周的刷新点
    auto cycle_start = target_time;

    // 检查当前时间是否已经过了本周的刷新点
    if (check_time < target_time)
    {
        // 如果还没过，说明当前仍属于上一周的“逻辑周”
        // 因此，正确的周期刷新点是上周的这个时间
        cycle_start = target_time - std::chrono::days(7);
    }

    // 将最终计算出的正确周期刷新点转换为毫秒时间戳
    return to_milliseconds(cycle_start);
}

int64_t celeritas::time_helper::get_start_of_month_milliseconds_with_offset(const time_point_type& check_time, const int64_t milliseconds_offset)
{
    // 获取本月1号的零点时间
    const auto month_start = get_start_of_month(check_time);

    if (milliseconds_offset == 0)
    {
        return to_milliseconds(month_start);
    }

    // 计算出本月的逻辑刷新点（本月1号零点 + 毫秒偏移）
    const auto target_time = month_start + std::chrono::milliseconds(milliseconds_offset);

    // 默认目标时间为本月的刷新点
    auto cycle_start = target_time;

    // 检查当前时间是否已经过了本月的刷新点
    if (check_time < target_time)
    {
        // 如果还没过，说明当前仍属于上一月的“逻辑月”
        // 因此，需要找到上个月的日期
        const auto last_month_start = get_start_of_month(check_time, -1);

        // 正确的周期刷新点是上个月的这个时间
        cycle_start = last_month_start + std::chrono::milliseconds(milliseconds_offset);
    }

    // 将最终计算出的正确周期刷新点转换为毫秒时间戳
    return to_milliseconds(cycle_start);
}

int64_t celeritas::time_helper::get_next_day_start_milliseconds_with_offset(const time_point_type& check_time, const int64_t milliseconds_offset)
{
    const auto day_start = get_start_of_day(check_time);
    const auto target_time = day_start + std::chrono::milliseconds(milliseconds_offset);

    auto next_cycle_start = target_time;
    if (check_time >= target_time)
    {
        next_cycle_start = target_time + std::chrono::days(1);
    }

    return to_milliseconds(next_cycle_start);
}

int64_t celeritas::time_helper::get_next_week_start_milliseconds_with_offset(const time_point_type& check_time, const int64_t milliseconds_offset)
{
    const auto week_start = get_start_of_week(check_time);
    const auto target_time = week_start + std::chrono::milliseconds(milliseconds_offset);

    auto next_cycle_start = target_time;
    if (check_time >= target_time)
    {
        next_cycle_start = target_time + std::chrono::days(7);
    }

    return to_milliseconds(next_cycle_start);
}

int64_t celeritas::time_helper::get_next_month_start_milliseconds_with_offset(const time_point_type& check_time, const int64_t milliseconds_offset)
{
    const auto month_start = get_start_of_month(check_time);
    const auto target_time = month_start + std::chrono::milliseconds(milliseconds_offset);

    auto next_cycle_start = target_time;
    if (check_time >= target_time)
    {
        const auto next_month_start = get_start_of_month(check_time, 1);
        next_cycle_start = next_month_start + std::chrono::milliseconds(milliseconds_offset);
    }

    return to_milliseconds(next_cycle_start);
}