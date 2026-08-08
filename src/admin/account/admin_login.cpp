// 创建时间：2026-07-30
// 修改时间：2026-07-30
// 审核时间：2026-08-07

#include "admin_login.h"
#include "admin_login_response.h"
#include "admin_status_type.h"
#include "admin/admin_constant.h"
#include "common/core/enum_cast.h"
#include "common/core/hmac_sha_256.h"
#include "common/core/time_helper.h"
#include "database/database_constant.h"
#include "database/generated/mysql/admin/admin_account.h"
#include "database/generated/redis/admin/admin_session_token.h"
#include "database/pool/database_pool_manager.h"
#include "detail/admin_login_parameter.h"

celeritas::admin_login::admin_login(http_handle_parameter_shared_ptr handle_parameter)
    : base_type{ std::move(handle_parameter) }
{
}

celeritas::admin_login::void_awaitable_type celeritas::admin_login::response()
{
    const admin_login_parameter parameter{ get_http_handle_parameter() };

    if (parameter.is_failure())
    {
        co_return co_await write_immediately(parameter.get_response());
    }

    const auto& username = parameter.get_username();
    const auto& password = parameter.get_password();
    const auto app_id = parameter.get_app_id();

    const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_admin_db_name.data());

    const auto optional_account = co_await load_account(mysql_pool, username, app_id);
    if (!optional_account)
    {
        co_return co_await write_immediately<admin_login_response>(game_error_type::password_error, "账号或密码错误");
    }

    auto account = *optional_account;

    if (hmac_sha256::calculate(password, account.get_salt()) != account.get_password_hash())
    {
        co_return co_await write_immediately<admin_login_response>(game_error_type::password_error, "账号或密码错误");
    }

    if (underlying_cast_enum<admin_status_type>(account.get_status()) == admin_status_type::disabled)
    {
        co_return co_await write_immediately<admin_login_response>(game_error_type::admin_account_disabled, "该账号已被禁用");
    }

    const auto client_ip = get_http_handle_parameter()->get_remote_ip_address();
    if (!co_await update_login_info(mysql_pool, account, client_ip))
    {
        co_return co_await write_immediately<admin_login_response>(game_error_type::mysql_error, "更新登录信息失败");
    }

    const auto optional_token = co_await create_session(account.get_account_id());
    if (!optional_token)
    {
        co_return co_await write_immediately<admin_login_response>(game_error_type::redis_error, "创建登录会话失败");
    }

    co_return co_await write_immediately<admin_login_response>(game_error_type::success,
                                                               "登录成功",
                                                               *optional_token,
                                                               std::to_string(account.get_account_id()),
                                                               account.get_username(),
                                                               account.get_role(),
                                                               account.get_avatar());
}

celeritas::http_service_base::void_awaitable_type celeritas::admin_login::send_error_response()
{
    co_return co_await write_immediately<admin_login_response>(game_error_type::unknown);
}

celeritas::admin_login::optional_admin_account_awaitable celeritas::admin_login::load_account(const database_pool_shared_ptr& mysql_pool,
                                                                                              const std::string& username,
                                                                                              const int64_t app_id)
{
    const auto key = std::make_shared<basis_database_container>(basis_database_container::object_container{ { admin_account::username_describe, username },
                                                                                                            { admin_account::app_id_describe, app_id } });

    const auto optional_admin_account = co_await mysql_pool->select_one(admin_account::get_select(database_type::mysql, key),
                                                                        admin_account::get_database_field_container());
    if (optional_admin_account)
    {
        co_return admin_account{ *optional_admin_account };
    }

    co_return std::nullopt;
}

celeritas::admin_login::bool_awaitable_type celeritas::admin_login::update_login_info(const database_pool_shared_ptr& mysql_pool,
                                                                                      admin_account& account,
                                                                                      const std::string& client_ip)
{
    account.set_last_login_time(time_helper::get_current_milliseconds());
    account.set_last_login_ip(client_ip);

    co_return co_await mysql_pool->execute_changes(account.get_modify());
}

celeritas::admin_login::optional_string_awaitable celeritas::admin_login::create_session(const int64_t account_id)
{
    const auto redis_pool = database_pool_manager::get_instance().get_pool(redis_db_name.data());
    const auto token = generate_token();

    admin_session_token session_token{ database_type::redis, token };
    session_token.set_token(token);
    session_token.set_account_id(account_id);

    if (co_await redis_pool->execute_changes(session_token.get_modify(), admin_session_ttl_seconds))
    {
        co_return token;
    }

    co_return std::nullopt;
}
