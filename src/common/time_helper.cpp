#include "common_fwd.h"
#include "logger.h"
#include "time_helper.h"

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

    // 计算本地时间的今天零点
    const auto today_midnight_local = get_current_day_local(now);

    // 将本地时间的零点转换回 system_clock::time_point
    const auto* current_zone = get_local_zone();
    const auto today_midnight = current_zone->to_sys(today_midnight_local);

    if (milliseconds_offset == 0)
    {
        return to_milliseconds(today_midnight);
    }

    // 计算今天日历上的逻辑刷新点（零点 + 毫秒偏移）
    const auto target_time_today = today_midnight + std::chrono::milliseconds(milliseconds_offset);

    // 默认目标时间为今天的刷新点
    auto cycle_target_time = target_time_today;

    // 检查当前时间是否已经过了今天的刷新点
    if (now < target_time_today)
    {
        // 如果还没过，说明当前仍属于前一天的“逻辑日”
        // 因此，正确的周期刷新点是昨天的这个时间
        cycle_target_time = target_time_today - std::chrono::days(1);
    }

    // 将最终计算出的正确周期刷新点转换为毫秒时间戳
    return to_milliseconds(cycle_target_time);
}

int64_t celeritas::time_helper::get_start_of_week_milliseconds()
{
    return get_start_of_week_milliseconds_with_offset(0);
}

int64_t celeritas::time_helper::get_start_of_week_milliseconds_with_offset(const int64_t milliseconds_offset)
{
    const auto now = std::chrono::system_clock::now();

    // 获取本地时间的今天日期
    const auto today_local = get_current_day_local(now);

    // 计算今天是星期几
    const std::chrono::weekday today_weekday{ today_local };

    // 计算需要回退多少天才能到本周的周一
    const auto days_to_subtract = std::chrono::days{ (today_weekday.c_encoding() + 6) % 7 };

    // 计算出本地时间的本周一
    const auto this_week_monday_local = today_local - days_to_subtract;

    // 将本地时间的本周一转换回 system_clock::time_point
    const auto* current_zone = get_local_zone();
    const auto this_week_monday = current_zone->to_sys(this_week_monday_local);

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
    if (now < target_time_this_week)
    {
        // 如果还没过，说明当前仍属于上一周的“逻辑周”
        // 因此，正确的周期刷新点是上周的这个时间
        cycle_target_time = target_time_this_week - std::chrono::days(7);
    }

    // 将最终计算出的正确周期刷新点转换为毫秒时间戳
    return to_milliseconds(cycle_target_time);
}

int64_t celeritas::time_helper::get_start_of_month_milliseconds()
{
    return get_start_of_month_milliseconds_with_offset(0);
}

int64_t celeritas::time_helper::get_start_of_month_milliseconds_with_offset(const int64_t milliseconds_offset)
{
    const auto now = std::chrono::system_clock::now();

    // 获取本地时间的今天日期
    const auto today_local = get_current_day_local(now);

    // 将日期转换为 year_month_day 格式
    const std::chrono::year_month_day year_month_day_local{ today_local };

    // 计算出本地时间的本月1号
    const auto this_month_first_day_local = std::chrono::year_month_day{ year_month_day_local.year(), year_month_day_local.month(), std::chrono::day{ 1 } };

    // 将本地时间的本月1号转换回 system_clock::time_point
    const auto* current_zone = get_local_zone();
    const auto this_month_first_day = current_zone->to_sys(std::chrono::local_days{ this_month_first_day_local });

    if (milliseconds_offset == 0)
    {
        return to_milliseconds(this_month_first_day);
    }

    // 计算出本月的逻辑刷新点（本月1号零点 + 毫秒偏移）
    const auto target_time_this_month = this_month_first_day + std::chrono::milliseconds(milliseconds_offset);

    // 默认目标时间为本月的刷新点
    auto cycle_target_time = target_time_this_month;

    // 检查当前时间是否已经过了本月的刷新点
    if (now < target_time_this_month)
    {
        // 如果还没过，说明当前仍属于上一月的“逻辑月”
        // 因此，需要找到上个月的日期
        const auto last_month_first_day_local = this_month_first_day_local - std::chrono::months(1);
        // 正确的周期刷新点是上个月的这个时间
        cycle_target_time = current_zone->to_sys(std::chrono::local_days{ last_month_first_day_local }) + std::chrono::milliseconds(milliseconds_offset);
    }

    // 将最终计算出的正确周期刷新点转换为毫秒时间戳
    return to_milliseconds(cycle_target_time);
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
        try
        {
            current_zone = std::chrono::get_tzdb().locate_zone("Asia/Shanghai");
            LOG_CHANNEL(celeritas::common_channel, warning) << "读不到系统本地时区，使用Asia/Shanghai。";
        }
        catch (const std::runtime_error& error)
        {
            LOG_CHANNEL(celeritas::common_channel, error) << "读不到Asia/Shanghai，使用Etc/UTC:" << error.what();
        }
    }
    else
    {
        LOG_CHANNEL(celeritas::common_channel, info) << "当前时区:" << current_zone->name();
    }

    return current_zone;
}

celeritas::time_helper::local_days_type celeritas::time_helper::get_current_day_local(const time_point_type& time_point)
{
    return std::chrono::floor<std::chrono::days>(to_local_time(time_point));
}
