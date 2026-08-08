// 创建时间：2026-07-27
// 修改时间：2026-07-28
// 审核时间：2026-07-28

#include "admin_delete.h"
#include "admin_delete_response.h"
#include "admin_role_type.h"
#include "admin/admin_constant.h"
#include "common/core/enum_cast.h"
#include "database/database_constant.h"
#include "database/generated/mysql/admin/admin_account.h"
#include "database/generated/redis/admin/admin_session_token.h"
#include "database/pool/database_pool_manager.h"
#include "detail/admin_delete_parameter.h"

celeritas::admin_delete::admin_delete(http_handle_parameter_shared_ptr handle_parameter)
    : base_type{ std::move(handle_parameter) }
{
}

celeritas::admin_delete::void_awaitable_type celeritas::admin_delete::response()
{
    const admin_delete_parameter parameter{ get_http_handle_parameter() };

    if (parameter.is_failure())
    {
        co_return co_await write_immediately(parameter.get_response());
    }

    const auto target_id = parameter.get_target_id();
    const auto app_id = parameter.get_app_id();
    const auto& token = parameter.get_token();

    const auto optional_operator_id = co_await resolve_operator_id(token);
    if (!optional_operator_id)
    {
        co_return co_await write_immediately<admin_delete_response>(game_error_type::token_error, "登录已失效，请重新登录");
    }

    const auto operator_account_id = *optional_operator_id;

    if (target_id == operator_account_id)
    {
        co_return co_await write_immediately<admin_delete_response>(game_error_type::cannot_delete_self, "无法删除当前登录的账号");
    }

    const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_admin_db_name.data());

    const auto optional_operator_account = co_await get_admin_account(mysql_pool, operator_account_id);
    if (!optional_operator_account)
    {
        co_return co_await write_immediately<admin_delete_response>(game_error_type::admin_account_not_found, "操作者账号未找到");
    }

    const auto optional_account = co_await load_target_account(mysql_pool, target_id, app_id);
    if (!optional_account)
    {
        co_return co_await write_immediately<admin_delete_response>(game_error_type::admin_account_not_found, "管理员账号未找到");
    }

    const auto& account = *optional_account;
    const auto operator_role = underlying_cast_enum<admin_role_type>(optional_operator_account->get_role());

    if (const auto target_role = underlying_cast_enum<admin_role_type>(account.get_role());
        !check_permission(operator_role, target_role, operator_account_id, account.get_creator_account_id()))
    {
        co_return co_await write_immediately<admin_delete_response>(game_error_type::insufficient_permissions, "权限不足，无法删除此账号");
    }

    // 禁止删除初始超级管理员
    if (account.get_username() == default_super_admin_username)
    {
        co_return co_await write_immediately<admin_delete_response>(game_error_type::cannot_delete_super_admin, "无法删除初始超级管理员账号");
    }

    if (co_await delete_account(mysql_pool, account))
    {
        co_return co_await write_immediately<admin_delete_response>(game_error_type::success, "管理员账号删除成功");
    }

    co_return co_await write_immediately<admin_delete_response>(game_error_type::mysql_error, "数据库操作失败");
}

celeritas::http_service_base::void_awaitable_type celeritas::admin_delete::send_error_response()
{
    co_return co_await write_immediately<admin_delete_response>(game_error_type::unknown);
}

celeritas::admin_delete::optional_admin_account_awaitable celeritas::admin_delete::load_target_account(const database_pool_shared_ptr& mysql_pool,
                                                                                                       const int64_t target_id,
                                                                                                       const int64_t app_id)
{
    const auto optional_admin_account = co_await get_admin_account(mysql_pool, target_id);
    if (!optional_admin_account || optional_admin_account->get_app_id() != app_id)
    {
        co_return std::nullopt;
    }
    co_return optional_admin_account;
}

celeritas::admin_delete::bool_awaitable_type celeritas::admin_delete::delete_account(const database_pool_shared_ptr& mysql_pool,
                                                                                     const admin_account& account)
{
    co_return co_await mysql_pool->execute_changes(account.get_delete());
}

bool celeritas::admin_delete::check_permission(const admin_role_type operator_role,
                                               const admin_role_type target_role,
                                               const int64_t operator_account_id,
                                               const int64_t creator_account_id)
{
    if (operator_role == admin_role_type::super_admin)
    {
        return true;
    }

    if (operator_role >= admin_role_type::operation_admin &&
        operator_role <= admin_role_type::developer_admin)
    {
        if (target_role == admin_role_type::readonly_user)
        {
            return true;
        }
    }

    return operator_account_id == creator_account_id;
}
