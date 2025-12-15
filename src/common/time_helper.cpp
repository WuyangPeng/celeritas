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
    return get_milliseconds_with_offset(0);
}

int64_t celeritas::time_helper::get_milliseconds_with_offset(const int64_t milliseconds_offset)
{
    // 获取当前时间点
    const auto now = std::chrono::system_clock::now();

    // 计算今天零点的时间点
    const auto today_midnight = std::chrono::floor<std::chrono::days>(now);

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
    // 获取当前时间点
    const auto now = std::chrono::system_clock::now();

    // 获取今天的日期（向下取整到天）
    const auto today = std::chrono::floor<std::chrono::days>(now);

    // 计算今天是星期几
    const std::chrono::year_month_day year_month_day{ today };
    const std::chrono::weekday today_weekday{ year_month_day };

    // 计算需要回退多少天才能到本周的周一
    const auto days_to_subtract = std::chrono::days{ (today_weekday.c_encoding() + 6) % 7 };

    // 计算出本周一的日期
    const auto this_week_monday = today - days_to_subtract;

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
    // 获取当前时间点
    const auto now = std::chrono::system_clock::now();

    // 获取今天的日期
    const auto today = std::chrono::floor<std::chrono::days>(now);

    // 将日期转换为 year_month_day 格式
    const std::chrono::year_month_day year_month_day{ today };

    // 计算出本月1号的日期
    const auto this_month_first_day = std::chrono::year_month_day{ year_month_day.year(), year_month_day.month(), std::chrono::day{ 1 } };

    // 计算出本月的逻辑刷新点（本月1号零点 + 毫秒偏移）
    const auto target_time_this_month = std::chrono::sys_days{ this_month_first_day } + std::chrono::milliseconds(milliseconds_offset);

    if (milliseconds_offset == 0)
    {
        // 将最终计算出的正确周期刷新点转换为毫秒时间戳
        return to_milliseconds(target_time_this_month);
    }

    // 默认目标时间为本月的刷新点
    auto cycle_target_time = target_time_this_month;

    // 检查当前时间是否已经过了本月的刷新点
    if (now < target_time_this_month)
    {
        // 如果还没过，说明当前仍属于上一月的“逻辑月”
        // 因此，需要找到上个月的日期
        const auto last_month_first_day = this_month_first_day - std::chrono::months(1);
        // 正确的周期刷新点是上个月的这个时间
        cycle_target_time = std::chrono::sys_days{ last_month_first_day } + std::chrono::milliseconds(milliseconds_offset);
    }

    // 将最终计算出的正确周期刷新点转换为毫秒时间戳
    return to_milliseconds(cycle_target_time);
}

int64_t celeritas::time_helper::to_milliseconds(const std::chrono::system_clock::time_point& time_point)
{
    return std::chrono::time_point_cast<std::chrono::milliseconds>(time_point).time_since_epoch().count();
}