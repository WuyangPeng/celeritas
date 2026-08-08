#include "admin/admin_constant.h"
#include "feedback_submit.h"
#include "common/core/snowflake_generator.h"
#include "common/core/time_helper.h"
#include "common/logging/logger.h"
#include "config/aggregate/app_config.h"
#include "database/database_constant.h"
#include "database/generated/mysql/admin/feedback.h"
#include "database/generated/redis/admin/feedback_limit.h"
#include "database/pool/database_pool_manager.h"
#include "detail/feedback_submit_parameter.h"
#include "message/basic/game_error_type.h"
#include "message/parameters/http_handle_parameter.h"

#include <boost/json.hpp>
#include <chrono>

namespace
{
    constexpr std::chrono::seconds feedback_limit_expiration_time{ 86400 };
    constexpr int32_t max_daily_feedback_count = 5;
}

celeritas::feedback_submit::feedback_submit(http_handle_parameter_shared_ptr handle_parameter)
    : base_type{ std::move(handle_parameter) }
{
}

celeritas::feedback_submit::void_awaitable_type celeritas::feedback_submit::response()
{
    feedback_submit_parameter parameter{ get_http_handle_parameter() };
    if (parameter.is_failure())
    {
        // 客户端反馈接口使用 status 字段，这里映射为兼容格式
        boost::json::object res_obj;
        const auto error = parameter.get_response();
        res_obj["status"] = static_cast<int>(error.get_code());
        res_obj["message"] = error.get_message();
        const std::string response_json = boost::json::serialize(res_obj);
        co_return co_await get_http_handle_parameter()->write_immediately(response_json);
    }

    const auto client_ip = get_http_handle_parameter()->get_remote_ip_address();
    const auto redis_pool = database_pool_manager::get_instance().get_pool(redis_db_name.data());

    int32_t current_count = 0;
    if (redis_pool)
    {
        if (auto limit_entity = co_await redis_pool->select_one(
            feedback_limit::get_select(database_type::redis, client_ip),
            feedback_limit::get_database_field_container()))
        {
            feedback_limit limit{ *limit_entity };
            current_count = limit.get_count();
            if (current_count >= max_daily_feedback_count)
            {
                boost::json::object res_obj;
                res_obj["status"] = static_cast<int>(game_error_type::feedback_daily_limit_exceeded);
                res_obj["message"] = get_game_error_description(game_error_type::feedback_daily_limit_exceeded);
                const std::string response_json = boost::json::serialize(res_obj);
                co_return co_await get_http_handle_parameter()->write_immediately(response_json);
            }
        }
    }

    boost::json::object res_obj;

    try
    {
        const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_admin_db_name.data());
        if (mysql_pool)
        {
            const int64_t now = time_helper::get_current_milliseconds();

            const auto server_config = get_app_config()->get_server_config();
            const auto feedback_id = snowflake_generator::get_instance().generate(
                server_config->get_datacenter_id(), server_config->get_worker_id());

            feedback new_item{ database_type::mysql, feedback_id };
            new_item.set_app_id(parameter.get_app_id());
            new_item.set_type(parameter.get_type());
            new_item.set_content(parameter.get_content());
            new_item.set_device_info(parameter.get_device_info());
            new_item.set_server(parameter.get_server());
            new_item.set_player_id(parameter.get_player_id());
            new_item.set_is_anonymous(parameter.is_anonymous());
            new_item.set_create_time(now);

            const auto& img_str = parameter.get_image_data();
            celeritas::traits::byte_array_type image_bytes(img_str.begin(), img_str.end());
            if (image_bytes.empty())
            {
                new_item.add_image_data(0);
                new_item.remove_image_data(0);
            }
            else
            {
                new_item.set_image_data(std::move(image_bytes));
            }

            if (co_await mysql_pool->execute_changes(new_item.get_modify()))
            {
                if (redis_pool)
                {
                    feedback_limit limit{ database_type::redis, client_ip };
                    limit.set_count(current_count + 1);
                    co_await redis_pool->execute_changes(limit.get_modify(), feedback_limit_expiration_time.count());
                }

                res_obj["status"] = static_cast<int>(game_error_type::success);
                res_obj["message"] = "呈送成功";
            }
            else
            {
                res_obj["status"] = static_cast<int>(game_error_type::mysql_error);
                res_obj["message"] = "数据库写入失败";
            }

            const std::string response_json = boost::json::serialize(res_obj);
            co_return co_await get_http_handle_parameter()->write_immediately(response_json);
        }
    }
    catch (const std::exception& e)
    {
        LOG_CHANNEL(celeritas::admin_channel, error) << "feedback submit error: " << e.what();
    }

    res_obj["status"] = static_cast<int>(game_error_type::server_error);
    res_obj["message"] = "服务器内部错误";
    const std::string response_json = boost::json::serialize(res_obj);
    co_return co_await get_http_handle_parameter()->write_immediately(response_json);
}

celeritas::http_service_base::void_awaitable_type celeritas::feedback_submit::send_error_response()
{
    boost::json::object res_obj;
    res_obj["status"] = static_cast<int>(game_error_type::server_error);
    res_obj["message"] = "服务器内部错误";

    const std::string response_json = boost::json::serialize(res_obj);
    co_return co_await get_http_handle_parameter()->write_immediately(response_json);
}
