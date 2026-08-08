// 创建时间：2026-08-06
// 修改时间：2026-08-06
// 审核时间：2026-08-06

#include "announcement_admin_create.h"
#include "announcement_admin_create_response.h"
#include "admin/admin_constant.h"
#include "common/core/enum_cast.h"
#include "common/core/snowflake_generator.h"
#include "common/core/time_helper.h"
#include "config/aggregate/app_config.h"
#include "database/database_constant.h"
#include "database/generated/mysql/admin/announcement.h"
#include "database/pool/database_pool_manager.h"
#include "detail/announcement_admin_create_parameter.h"

celeritas::announcement_admin_create::announcement_admin_create(http_handle_parameter_shared_ptr handle_parameter)
    : base_type{ std::move(handle_parameter) }
{
}

celeritas::announcement_admin_create::void_awaitable_type celeritas::announcement_admin_create::response()
{
    const announcement_admin_create_parameter parameter{ get_http_handle_parameter() };

    if (parameter.is_failure())
    {
        co_return co_await write_immediately(parameter.get_response());
    }

    const auto optional_operator_id = co_await resolve_operator_id(parameter.get_token());
    if (!optional_operator_id)
    {
        co_return co_await write_immediately<announcement_admin_create_response>(game_error_type::token_error, "登录已失效，请重新登录");
    }

    const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_admin_db_name.data());

    const auto now = time_helper::get_current_milliseconds();
    const auto server_config = get_app_config()->get_server_config();
    const auto new_id = snowflake_generator::get_instance().generate(server_config->get_datacenter_id(), server_config->get_worker_id());

    announcement new_item{ database_type::mysql, new_id };
    new_item.set_app_id(parameter.get_app_id());
    new_item.set_title(parameter.get_title());
    new_item.set_tag(parameter.get_tag());
    new_item.set_banner_url(parameter.get_banner_url());
    new_item.set_content(parameter.get_content());
    new_item.set_priority(parameter.get_priority());
    new_item.set_publish_time(parameter.get_publish_time());
    new_item.set_status(enum_cast_underlying(parameter.get_status()));
    new_item.set_server(parameter.get_server());
    new_item.set_create_time(now);
    new_item.set_update_time(now);

    if (co_await mysql_pool->execute_changes(new_item.get_modify()))
    {
        co_return co_await write_immediately<announcement_admin_create_response>(game_error_type::success,
                                                                                 "创建公告成功",
                                                                                 new_id,
                                                                                 parameter.get_title(),
                                                                                 parameter.get_status());
    }

    co_return co_await write_immediately<announcement_admin_create_response>(game_error_type::mysql_error, "数据库写入失败");
}

celeritas::http_service_base::void_awaitable_type celeritas::announcement_admin_create::send_error_response()
{
    co_return co_await write_immediately<announcement_admin_create_response>(game_error_type::unknown);
}
