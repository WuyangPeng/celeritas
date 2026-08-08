// 创建时间：2026-07-26
// 修改时间：2026-07-31
// 审核时间：2026-07-31

#include "admin_create.h"
#include "admin_create_response.h"
#include "admin_status_type.h"
#include "admin/admin_constant.h"
#include "admin/account/admin_role_type.h"
#include "common/core/enum_cast.h"
#include "common/core/hmac_sha_256.h"
#include "common/core/snowflake_generator.h"
#include "common/core/time_helper.h"
#include "common/logging/logger.h"
#include "config/aggregate/app_config.h"
#include "database/database_constant.h"
#include "database/generated/mysql/admin/admin_account.h"
#include "database/generated/redis/admin/admin_session_token.h"
#include "database/pool/database_pool_manager.h"
#include "detail/admin_create_parameter.h"

celeritas::admin_create::admin_create(http_handle_parameter_shared_ptr handle_parameter)
    : base_type{ std::move(handle_parameter) }
{
}

celeritas::admin_create::void_awaitable_type celeritas::admin_create::response()
{
    const admin_create_parameter parameter{ get_http_handle_parameter() };

    if (parameter.is_failure())
    {
        co_return co_await write_immediately(parameter.get_response());
    }

    const auto& username = parameter.get_username();
    const auto app_id = parameter.get_app_id();
    const auto& token = parameter.get_token();

    const auto target_role = parameter.get_role();

    const auto optional_operator_id = co_await resolve_operator_id(token);
    if (!optional_operator_id)
    {
        co_return co_await write_immediately<admin_create_response>(game_error_type::token_error, "登录已失效，请重新登录");
    }

    const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_admin_db_name.data());

    const auto optional_operator_account = co_await get_admin_account(mysql_pool, *optional_operator_id);
    if (!optional_operator_account)
    {
        co_return co_await write_immediately<admin_create_response>(game_error_type::admin_account_not_found, "操作者账号未找到");
    }

    if (const auto operator_role = underlying_cast_enum<admin_role_type>(optional_operator_account->get_role());
        !check_permission(operator_role, target_role))
    {
        co_return co_await write_immediately<admin_create_response>(game_error_type::insufficient_permissions, "权限不足，无法创建此级别的账号");
    }

    if (co_await check_username_exists(mysql_pool, username, app_id))
    {
        co_return co_await write_immediately<admin_create_response>(game_error_type::username_already_exists, "用户名已存在");
    }

    const auto server_config = get_app_config()->get_server_config();

    if (const auto account_id = snowflake_generator::get_instance().generate(server_config->get_datacenter_id(), server_config->get_worker_id());
        co_await create_and_save_account(mysql_pool, parameter, account_id, *optional_operator_id))
    {
        co_return co_await write_immediately<admin_create_response>(game_error_type::success, "管理员账号创建成功", account_id, username);
    }

    co_return co_await write_immediately<admin_create_response>(game_error_type::mysql_error, "数据库写入失败");
}

celeritas::http_service_base::void_awaitable_type celeritas::admin_create::send_error_response()
{
    co_return co_await write_immediately<admin_create_response>(game_error_type::unknown);
}

celeritas::admin_create::bool_awaitable_type celeritas::admin_create::check_username_exists(const database_pool_shared_ptr& mysql_pool,
                                                                                            const std::string& username,
                                                                                            int64_t app_id)
{
    const auto key = std::make_shared<basis_database_container>(basis_database_container::object_container{ { admin_account::username_describe, username },
                                                                                                            { admin_account::app_id_describe, app_id } });

    co_return co_await mysql_pool->select_one(admin_account::get_select(database_type::mysql, key),
                                              admin_account::get_database_field_container());
}

celeritas::admin_create::bool_awaitable_type celeritas::admin_create::create_and_save_account(const database_pool_shared_ptr& mysql_pool,
                                                                                              const admin_create_parameter& parameter,
                                                                                              const int64_t account_id,
                                                                                              const int64_t creator_id)
{
    const auto& username = parameter.get_username();
    const auto app_id = parameter.get_app_id();
    const auto& password = parameter.get_password();
    const auto role = parameter.get_role();
    const auto& nickname = parameter.get_nickname();
    const auto& avatar = parameter.get_avatar();

    const auto salt = generate_token();
    const auto hashed_password = hmac_sha256::calculate(password, salt);
    const auto now_timestamp = time_helper::get_current_milliseconds();

    admin_account admin_account{ database_type::mysql, account_id };
    admin_account.set_username(username);
    admin_account.set_app_id(app_id);
    admin_account.set_password_hash(hashed_password);
    admin_account.set_salt(salt);
    admin_account.set_role(enum_cast_underlying(role));
    admin_account.set_nickname(nickname);
    admin_account.set_avatar(avatar);
    admin_account.set_creator_account_id(creator_id);
    admin_account.set_status(enum_cast_underlying(admin_status_type::enabled));
    admin_account.set_create_time(now_timestamp);
    admin_account.set_update_time(now_timestamp);

    co_return co_await mysql_pool->execute_changes(admin_account.get_modify());
}

bool celeritas::admin_create::check_permission(const admin_role_type operator_role,
                                               const admin_role_type target_role)
{
    if (operator_role == admin_role_type::super_admin)
    {
        return true;
    }

    if (operator_role >= admin_role_type::operation_admin &&
        operator_role <= admin_role_type::developer_admin)
    {
        return target_role >= operator_role;
    }

    return false;
}
