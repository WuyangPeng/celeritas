#include "announcement_red_dot.h"
#include "admin/admin_constant.h"
#include "admin/announcement/announcement_type.h"
#include "common/core/enum_cast.h"
#include "common/logging/logger.h"
#include "database/database_constant.h"
#include "database/generated/mysql/admin/announcement.h"
#include "database/pool/database_pool_manager.h"
#include "detail/announcement_red_dot_parameter.h"
#include "message/parameters/http_handle_parameter.h"

#include <boost/json.hpp>
#include <algorithm>

celeritas::announcement_red_dot::announcement_red_dot(http_handle_parameter_shared_ptr handle_parameter)
    : base_type{ std::move(handle_parameter) }
{
}

celeritas::announcement_red_dot::void_awaitable_type celeritas::announcement_red_dot::response()
{
    // 在执行任何 co_await 挂起操作之前，同步完成参数解析与签名校验，防止内存视图在协程挂起期间被释放
    announcement_red_dot_parameter parameter{ get_http_handle_parameter() };
    if (parameter.is_failure())
    {
        co_return co_await write_immediately(parameter.get_response());
    }

    const int64_t last_time = parameter.get_last_time();
    boost::json::object res_obj;

    try
    {
        const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_admin_db_name.data());
        if (mysql_pool)
        {
            const auto results = co_await mysql_pool->select_all<announcement>(database_type::mysql);

            std::vector<announcement> all_items;
            for (const auto& change : results)
            {
                all_items.emplace_back(change);
            }

            int red_dot_count = 0;
            for (const auto& item : all_items)
            {
                // 只统计已发布公告中，update_time 晚于玩家上次阅读时刻的条目。
                // 使用 update_time 而非 id，是因为旧公告被隐藏后重新激活时
                // id 不变，但 update_time 会更新，可以正确触发红点。
                if (item.get_app_id() == parameter.get_app_id() &&
                    item.get_status() == enum_cast_underlying(announcement_status::published) &&
                    item.get_update_time() > last_time)
                {
                    red_dot_count++;
                }
            }

            res_obj["code"] = 0;
            res_obj["message"] = "获取成功";

            boost::json::object data_obj;
            data_obj["red_dot_count"] = red_dot_count;
            res_obj["data"] = data_obj;

            const std::string response_json = boost::json::serialize(res_obj);
            co_return co_await get_http_handle_parameter()->write_immediately(response_json);
        }
    }
    catch (const std::exception& e)
    {
        LOG_CHANNEL(celeritas::admin_channel, error) << "announcement red dot get error: " << e.what();
    }

    res_obj["code"] = 500;
    res_obj["message"] = "服务器内部错误";
    const std::string response_json = boost::json::serialize(res_obj);
    co_return co_await get_http_handle_parameter()->write_immediately(response_json);
}

celeritas::http_service_base::void_awaitable_type celeritas::announcement_red_dot::send_error_response()
{
    boost::json::object res_obj;
    res_obj["code"] = 500;
    res_obj["message"] = "服务器内部错误";

    const std::string response_json = boost::json::serialize(res_obj);
    co_return co_await get_http_handle_parameter()->write_immediately(response_json);
}
