#include "admin/admin_constant.h"
#include "admin/announcement/announcement_type.h"
#include "announcement_client.h"
#include "common/core/enum_cast.h"
#include "common/logging/logger.h"
#include "database/database_constant.h"
#include "database/generated/mysql/admin/announcement.h"
#include "database/pool/database_pool_manager.h"
#include "detail/announcement_client_parameter.h"
#include "message/parameters/http_handle_parameter.h"


#include <boost/json.hpp>
#include <algorithm>

celeritas::announcement_client::announcement_client(http_handle_parameter_shared_ptr handle_parameter)
    : base_type{ std::move(handle_parameter) }
{
}

celeritas::announcement_client::void_awaitable_type celeritas::announcement_client::response()
{
    announcement_client_parameter parameter{ get_http_handle_parameter() };
    if (parameter.is_failure())
    {
        co_return co_await write_immediately(parameter.get_response());
    }

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

            // 只返回 published 状态且属于当前 app 的公告，按 priority 降序
            std::vector<announcement*> published;
            for (auto& item : all_items)
            {
                if (item.get_app_id() == parameter.get_app_id() &&
                    item.get_status() == enum_cast_underlying(announcement_status::published))
                    published.push_back(&item);
            }

            std::sort(published.begin(), published.end(), [](const announcement* a, const announcement* b) {
                if (a->get_priority() != b->get_priority())
                    return a->get_priority() > b->get_priority();
                return a->get_publish_time() > b->get_publish_time();
            });

            boost::json::array list_arr;
            for (const auto* item : published)
            {
                boost::json::object obj;
                obj[announcement_id_describe] = item->get_id();
                obj[announcement_title_describe] = item->get_title();
                obj[announcement_tag_describe] = item->get_tag();
                obj[announcement_banner_url_describe] = item->get_banner_url();
                obj[announcement_content_describe] = item->get_content();
                obj[announcement_priority_describe] = item->get_priority();
                obj[announcement_publish_time_describe] = item->get_publish_time();
                list_arr.push_back(obj);
            }

            res_obj["code"] = 0;
            res_obj["message"] = "获取成功";
            res_obj["data"] = list_arr;
            res_obj["announcements"] = list_arr;

            const std::string response_json = boost::json::serialize(res_obj);
            co_return co_await get_http_handle_parameter()->write_immediately(response_json);
        }
    }
    catch (const std::exception& e)
    {
        LOG_CHANNEL(celeritas::admin_channel, error) << "announcement client get error: " << e.what();
    }

    res_obj["code"] = 500;
    res_obj["message"] = "服务器内部错误";
    const std::string response_json = boost::json::serialize(res_obj);
    co_return co_await get_http_handle_parameter()->write_immediately(response_json);
}

celeritas::http_service_base::void_awaitable_type celeritas::announcement_client::send_error_response()
{
    boost::json::object res_obj;
    res_obj["code"] = 500;
    res_obj["message"] = "服务器内部错误";

    const std::string response_json = boost::json::serialize(res_obj);
    co_return co_await get_http_handle_parameter()->write_immediately(response_json);
}
