#include "auth_login.h"
#include "auth/authentication/sdk/sdk_process_type.h"
#include "../../common/core_utilities/celeritas_error.h"
#include "common/hmac_sha_256.h"
#include "common/snowflake_generator.h"
#include "common/time_helper.h"
#include "config/app_config.h"
#include "database/database_pool_base.h"
#include "database/generated/mysql/auth/account_bind.h"
#include "initializer/account_status_type.h"

celeritas::auth_login::auth_login(http_handle_parameter_shared_ptr handle_parameter)
    : base_type{ std::move(handle_parameter) }, account_bind_id_{}
{
}

celeritas::auth_login::optional_database_entity_change_awaitable_type celeritas::auth_login::get_optional_account(int64_t app_id,
                                                                                                                  const std::string& device_id,
                                                                                                                  const database_pool_shared_ptr& mysql_pool)
{
    const auto key = std::make_shared<basis_database_container>(basis_database_container::object_container{ { account::device_id_describe, device_id },
                                                                                                            { account::app_id_describe, app_id } });

    co_return co_await mysql_pool->select_one(account::get_select(database_type::mysql, key),
                                              account::get_database_field_container());
}

celeritas::auth_login::account_awaitable_type celeritas::auth_login::create_new_account(const int64_t app_id,
                                                                                        const std::string& device_id,
                                                                                        const database_pool_shared_ptr& redis_pool,
                                                                                        const const_app_config_shared_ptr& app_config)
{
    const auto server_config = app_config->get_server_config();
    const auto account_id = snowflake_generator::get_instance().generate(server_config.get_datacenter_id(), server_config.get_worker_id());

    // 账号只存入redis，等待玩家真正登陆时再写入mysql
    account account{ database_type::redis, account_id };

    account.set_account_name("guest_" + std::to_string(account_id));
    account.set_device_id(device_id);
    account.set_app_id(app_id);
    account.set_create_time(time_helper::get_current_milliseconds());
    account.set_status(static_cast<int>(account_status_type::normal));

    if (co_await redis_pool->execute_changes(account.get_modify()))
    {
        co_return account;
    }

    throw celeritas_error{ "guest login error" };
}

celeritas::auth_login::session_token_awaitable_type celeritas::auth_login::create_session_token(const account& account,
                                                                                                const bool is_new_account,
                                                                                                const database_pool_shared_ptr& redis_pool)
{
    const auto token = generate_token();

    session_token session_token{ database_type::redis, token };
    session_token.set_token(token);
    session_token.set_account_id(account.get_account_id());
    session_token.set_new_account(is_new_account);
    session_token.set_account_bind_id(account_bind_id_);

    // 这里没有删除旧的token，旧的token依赖redis有效时间进行删除。
    if (co_await redis_pool->execute_changes(session_token.get_modify()))
    {
        co_return session_token;
    }

    co_return std::nullopt;
}

celeritas::auth_login::account_awaitable_type celeritas::auth_login::create_new_account(const int64_t app_id,
                                                                                        const std::string& auth_key,
                                                                                        const account_type account_type,
                                                                                        const sdk_process_type sdk_process_type,
                                                                                        const std::string& account_name_prefix,
                                                                                        const database_pool_shared_ptr& redis_pool,
                                                                                        const const_app_config_shared_ptr& app_config)
{
    co_return co_await create_new_account(app_id, auth_key, "", account_type, sdk_process_type, account_name_prefix, redis_pool, app_config);
}

celeritas::auth_login::account_awaitable_type celeritas::auth_login::create_new_account(int64_t app_id,
                                                                                        const std::string& auth_key,
                                                                                        const std::string& password,
                                                                                        account_type account_type,
                                                                                        const sdk_process_type sdk_process_type,
                                                                                        const std::string& account_name_prefix,
                                                                                        const database_pool_shared_ptr& redis_pool,
                                                                                        const const_app_config_shared_ptr& app_config)
{
    const auto server_config = app_config->get_server_config();
    const auto account_id = snowflake_generator::get_instance().generate(server_config.get_datacenter_id(), server_config.get_worker_id());

    // 账号只存入redis，等待玩家真正登陆时再写入mysql
    account account{ database_type::redis, account_id };
    account.set_account_name(get_sdk_process_type_description(sdk_process_type) + account_name_prefix + "_" + std::to_string(account_id));
    account.set_create_time(time_helper::get_current_milliseconds());
    account.set_status(static_cast<int>(account_status_type::normal));
    account.set_device_id(account.get_account_name());
    account.set_app_id(app_id);

    if (!password.empty())
    {
        const auto salt = generate_token();
        const auto hashed_password = hmac_sha256::calculate(password, salt);
        account.set_salt(salt);
        account.set_password_hash(hashed_password);
    }

    account_bind_id_ = snowflake_generator::get_instance().generate(server_config.get_datacenter_id(), server_config.get_worker_id());
    account_bind account_bind{ database_type::redis, account_bind_id_ };
    account_bind.set_account_id(account_id);
    account_bind.set_auth_key(auth_key);
    account_bind.set_account_type(static_cast<int>(account_type));
    account_bind.set_app_id(app_id);
    account_bind.set_process_type(static_cast<int>(sdk_process_type));

    if (co_await redis_pool->execute_changes(account.get_modify()) &&
        co_await redis_pool->execute_changes(account_bind.get_modify()))
    {
        co_return account;
    }

    throw celeritas_error{ account_name_prefix + " login error" };
}