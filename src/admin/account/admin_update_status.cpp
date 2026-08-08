// 创建时间：2026-07-30
// 修改时间：2026-07-30
// 审核时间：2026-08-07

#include "admin_update_status.h"
#include "admin_update_status_response.h"
#include "admin_role_type.h"
#include "admin_status_type.h"
#include "admin/admin_constant.h"
#include "common/core/enum_cast.h"
#include "common/core/time_helper.h"
#include "database/database_constant.h"
#include "database/generated/mysql/admin/admin_account.h"
#include "database/generated/redis/admin/admin_session_token.h"
#include "database/pool/database_pool_manager.h"
#include "detail/admin_update_status_parameter.h"

celeritas::admin_update_status::admin_update_status(http_handle_parameter_shared_ptr handle_parameter)
    : base_type{ std::move(handle_parameter) }
{
}

celeritas::admin_update_status::void_awaitable_type celeritas::admin_update_status::response()
{
    const admin_update_status_parameter parameter{ get_http_handle_parameter() };

    if (parameter.is_failure())
    {
        co_return co_await write_immediately(parameter.get_response());
    }

    const auto target_id = parameter.get_target_id();
    const auto app_id = parameter.get_app_id();
    const auto status = parameter.get_status();
    const auto& token = parameter.get_token();

    const auto optional_operator_id = co_await resolve_operator_id(token);
    if (!optional_operator_id)
    {
        co_return co_await write_immediately<admin_update_status_response>(game_error_type::token_error, "登录已失效，请重新登录");
    }

    const auto operator_account_id = *optional_operator_id;

    if (target_id == operator_account_id)
    {
        co_return co_await write_immediately<admin_update_status_response>(game_error_type::cannot_update_self_status, "无法修改当前登录账号的状态");
    }

    const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_admin_db_name.data());

    const auto optional_operator_account = co_await get_admin_account(mysql_pool, operator_account_id);
    if (!optional_operator_account)
    {
        co_return co_await write_immediately<admin_update_status_response>(game_error_type::admin_account_not_found, "操作者账号未找到");
    }

    const auto optional_account = co_await load_target_account(mysql_pool, target_id, app_id);
    if (!optional_account)
    {
        co_return co_await write_immediately<admin_update_status_response>(game_error_type::admin_account_not_found, "管理员账号未找到");
    }

    auto account = *optional_account;

    if (account.get_username() == default_super_admin_username)
    {
        co_return co_await write_immediately<admin_update_status_response>(game_error_type::cannot_disable_super_admin, "无法修改初始超级管理员账号状态");
    }

    const auto operator_role = underlying_cast_enum<admin_role_type>(optional_operator_account->get_role());
    const auto target_role = underlying_cast_enum<admin_role_type>(account.get_role());

    if (!check_permission(operator_role, target_role, operator_account_id, account.get_creator_account_id()))
    {
        co_return co_await write_immediately<admin_update_status_response>(game_error_type::insufficient_permissions, "权限不足，无法修改此账号状态");
    }

    if (co_await update_status(mysql_pool, account, status))
    {
        co_return co_await write_immediately<admin_update_status_response>(game_error_type::success, "账号状态更新成功");
    }

    co_return co_await write_immediately<admin_update_status_response>(game_error_type::mysql_error, "数据库更新失败");
}

celeritas::http_service_base::void_awaitable_type celeritas::admin_update_status::send_error_response()
{
    co_return co_await write_immediately<admin_update_status_response>(game_error_type::unknown);
}

celeritas::admin_update_status::optional_admin_account_awaitable celeritas::admin_update_status::load_target_account(const database_pool_shared_ptr& mysql_pool,
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

celeritas::admin_update_status::bool_awaitable_type celeritas::admin_update_status::update_status(const database_pool_shared_ptr& mysql_pool,
                                                                                                  admin_account& account,
                                                                                                  const admin_status_type status)
{
    account.set_status(enum_cast_underlying(status));
    account.set_update_time(time_helper::get_current_milliseconds());

    co_return co_await mysql_pool->execute_changes(account.get_modify());
}

bool celeritas::admin_update_status::check_permission(const admin_role_type operator_role,
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
