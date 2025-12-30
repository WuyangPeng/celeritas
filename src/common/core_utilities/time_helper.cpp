#include "time_helper.h"
#include "common/common_fwd.h"
#include "common/logger.h"

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

    const auto* current_zone = get_local_zone();
    return current_zone->to_sys(local_day);
}

celeritas::time_helper::time_point_type celeritas::time_helper::get_start_of_week(const time_point_type& time_point)
{
    const auto local_day = get_local_day(time_point);

    const std::chrono::weekday today_weekday{ local_day };
    const auto days_to_subtract = std::chrono::days{ (today_weekday.c_encoding() + 6) % 7 };
    const auto week_local_day = local_day - days_to_subtract;

    const auto* current_zone = get_local_zone();
    return current_zone->to_sys(week_local_day);
}

celeritas::time_helper::time_point_type celeritas::time_helper::get_start_of_month(const time_point_type& time_point)
{
    const auto local_day = get_local_day(time_point);

    const std::chrono::year_month_day today_year_month_day{ local_day };
    const auto this_month_first_day = std::chrono::year_month_day{ today_year_month_day.year(), today_year_month_day.month(), std::chrono::day{ 1 } };

    const auto* current_zone = get_local_zone();
    return current_zone->to_sys(std::chrono::local_days{ this_month_first_day });
}

int64_t celeritas::time_helper::get_start_of_day_milliseconds_with_offset(const time_point_type& check_time, const int64_t milliseconds_offset)
{
    // 获取当天的零点时间
    const auto today_midnight = get_start_of_day(check_time);

    if (milliseconds_offset == 0)
    {
        return to_milliseconds(today_midnight);
    }

    // 计算今天日历上的逻辑刷新点（零点 + 毫秒偏移）
    const auto target_time_today = today_midnight + std::chrono::milliseconds(milliseconds_offset);

    // 默认目标时间为今天的刷新点
    auto cycle_target_time = target_time_today;

    // 检查当前时间是否已经过了今天的刷新点
    if (check_time < target_time_today)
    {
        // 如果还没过，说明当前仍属于前一天的“逻辑日”
        // 因此，正确的周期刷新点是昨天的这个时间
        cycle_target_time = target_time_today - std::chrono::days(1);
    }

    // 将最终计算出的正确周期刷新点转换为毫秒时间戳
    return to_milliseconds(cycle_target_time);
}

int64_t celeritas::time_helper::get_start_of_week_milliseconds_with_offset(const time_point_type& check_time, const int64_t milliseconds_offset)
{
    // 获取本周一的零点时间
    const auto this_week_monday = get_start_of_week(check_time);

    if (milliseconds_offset == 0)
    {
        // 将本周一的零点时间点转换为毫秒时间戳并返回
        return to_milliseconds(this_week_monday);
    }

    // 计算出本周的逻辑刷新点（周一零点 + 毫秒偏移）
    const auto target_time_this_week = this_week_monday + std::chrono::milliseconds(milliseconds_offset);

    // 默认目标时间为本周的刷新点
    auto cycle_target_time = target_time_this_week;

    // 检查当前时间是否已经过了本周的刷新点
    if (check_time < target_time_this_week)
    {
        // 如果还没过，说明当前仍属于上一周的“逻辑周”
        // 因此，正确的周期刷新点是上周的这个时间
        cycle_target_time = target_time_this_week - std::chrono::days(7);
    }

    // 将最终计算出的正确周期刷新点转换为毫秒时间戳
    return to_milliseconds(cycle_target_time);
}

int64_t celeritas::time_helper::get_start_of_month_milliseconds_with_offset(const time_point_type& check_time, const int64_t milliseconds_offset)
{
    // 获取本月1号的零点时间
    const auto this_month_first_day = get_start_of_month(check_time);

    if (milliseconds_offset == 0)
    {
        return to_milliseconds(this_month_first_day);
    }

    // 计算出本月的逻辑刷新点（本月1号零点 + 毫秒偏移）
    const auto target_time_this_month = this_month_first_day + std::chrono::milliseconds(milliseconds_offset);

    // 默认目标时间为本月的刷新点
    auto cycle_target_time = target_time_this_month;

    // 检查当前时间是否已经过了本月的刷新点
    if (check_time < target_time_this_month)
    {
        // 如果还没过，说明当前仍属于上一月的“逻辑月”
        // 因此，需要找到上个月的日期
        cycle_target_time = get_last_of_month_milliseconds(check_time, milliseconds_offset);
    }

    // 将最终计算出的正确周期刷新点转换为毫秒时间戳
    return to_milliseconds(cycle_target_time);
}

celeritas::time_helper::time_point_type celeritas::time_helper::get_last_of_month_milliseconds(const time_point_type& check_time, const int64_t milliseconds_offset)
{
    const auto local_day = get_local_day(check_time);
    const std::chrono::year_month_day local_year_month_day{ local_day };
    const auto this_month_first_day = std::chrono::year_month_day{ local_year_month_day.year(), local_year_month_day.month(), std::chrono::day{ 1 } };
    const auto last_month_first_day = this_month_first_day - std::chrono::months(1);

    const auto* current_zone = get_local_zone();
    // 正确的周期刷新点是上个月的这个时间
    return current_zone->to_sys(std::chrono::local_days{ last_month_first_day }) + std::chrono::milliseconds(milliseconds_offset);
}

int64_t celeritas::time_helper::get_next_day_start_milliseconds_with_offset(const time_point_type& check_time, const int64_t milliseconds_offset)
{
    const auto today_midnight = get_start_of_day(check_time);
    const auto target_time_today = today_midnight + std::chrono::milliseconds(milliseconds_offset);

    auto end_of_cycle_time = target_time_today;
    if (check_time >= target_time_today)
    {
        end_of_cycle_time = target_time_today + std::chrono::days(1);
    }

    return to_milliseconds(end_of_cycle_time);
}

int64_t celeritas::time_helper::get_next_week_start_milliseconds_with_offset(const time_point_type& check_time, const int64_t milliseconds_offset)
{
    const auto this_week_monday = get_start_of_week(check_time);
    const auto target_time_this_week = this_week_monday + std::chrono::milliseconds(milliseconds_offset);

    auto end_of_cycle_time = target_time_this_week;
    if (check_time >= target_time_this_week)
    {
        end_of_cycle_time = target_time_this_week + std::chrono::days(7);
    }

    return to_milliseconds(end_of_cycle_time);
}

int64_t celeritas::time_helper::get_next_month_start_milliseconds_with_offset(const time_point_type& check_time, const int64_t milliseconds_offset)
{
    const auto this_month_first_day = get_start_of_month(check_time);
    const auto target_time_this_month = this_month_first_day + std::chrono::milliseconds(milliseconds_offset);

    auto end_of_cycle_time = target_time_this_month;
    if (check_time >= target_time_this_month)
    {
        const auto today_local = get_local_day(check_time);
        const std::chrono::year_month_day year_month_day_local{ today_local };
        const auto this_month_first_day_local = std::chrono::year_month_day{ year_month_day_local.year(), year_month_day_local.month(), std::chrono::day{ 1 } };
        const auto next_month_first_day_local = this_month_first_day_local + std::chrono::months(1);
        const auto* current_zone = get_local_zone();
        end_of_cycle_time = current_zone->to_sys(std::chrono::local_days{ next_month_first_day_local }) + std::chrono::milliseconds(milliseconds_offset);
    }

    return to_milliseconds(end_of_cycle_time);
}
