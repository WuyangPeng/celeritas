// 创建时间：2026-08-06
// 修改时间：2026-08-06
// 审核时间：2026-08-06

#include "announcement_admin_delete.h"
#include "announcement_admin_delete_response.h"
#include "admin/admin_constant.h"
#include "database/database_constant.h"
#include "database/generated/mysql/admin/announcement.h"
#include "database/pool/database_pool_manager.h"
#include "detail/announcement_admin_delete_parameter.h"

celeritas::announcement_admin_delete::announcement_admin_delete(http_handle_parameter_shared_ptr handle_parameter)
    : base_type{ std::move(handle_parameter) }
{
}

celeritas::announcement_admin_delete::void_awaitable_type celeritas::announcement_admin_delete::response()
{
    const announcement_admin_delete_parameter parameter{ get_http_handle_parameter() };

    if (parameter.is_failure())
    {
        co_return co_await write_immediately(parameter.get_response());
    }

    if (const auto optional_operator_id = co_await resolve_operator_id(parameter.get_token()); !optional_operator_id)
    {
        co_return co_await write_immediately<announcement_admin_delete_response>(game_error_type::token_error, "登录已失效，请重新登录");
    }

    const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_admin_db_name.data());

    const auto id = parameter.get_id();
    const auto optional_item = co_await mysql_pool->select_one(announcement::get_select(database_type::mysql, id),
                                                               announcement::get_database_field_container());

    if (!optional_item)
    {
        co_return co_await write_immediately<announcement_admin_delete_response>(game_error_type::announcement_not_found, "公告不存在");
    }

    announcement item{ *optional_item };
    if (item.get_app_id() != parameter.get_app_id())
    {
        co_return co_await write_immediately<announcement_admin_delete_response>(game_error_type::announcement_not_found, "公告不存在");
    }

    if (co_await mysql_pool->execute_changes(item.get_delete()))
    {
        co_return co_await write_immediately<announcement_admin_delete_response>(game_error_type::success, "删除成功");
    }

    co_return co_await write_immediately<announcement_admin_delete_response>(game_error_type::mysql_error, "数据库删除失败");
}

celeritas::http_service_base::void_awaitable_type celeritas::announcement_admin_delete::send_error_response()
{
    co_return co_await write_immediately<announcement_admin_delete_response>(game_error_type::unknown);
}


