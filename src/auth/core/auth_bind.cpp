#include "auth_bind.h"
#include "common/hmac_sha_256.h"
#include "common/core_utilities/snowflake_generator.h"
#include "config/app_config.h"
#include "database/database_pool_base.h"
#include "database/generated/mysql/auth/account_bind.h"

celeritas::auth_bind::auth_bind(http_handle_parameter_shared_ptr handle_parameter)
    : base_type{ std::move(handle_parameter) }
{
}

celeritas::auth_bind::bool_awaitable_type celeritas::auth_bind::bind(account& account,
                                                                     const int64_t app_id,
                                                                     const std::string& auth_key,
                                                                     const account_type account_type,
                                                                     const sdk_process_type sdk_process_type,
                                                                     const database_pool_shared_ptr& mysql_pool) const
{
    co_return co_await bind(account, app_id, auth_key, "", account_type, sdk_process_type, mysql_pool);
}

celeritas::auth_bind::bool_awaitable_type celeritas::auth_bind::bind(account& account,
                                                                     const int64_t app_id,
                                                                     const std::string& auth_key,
                                                                     const std::string& password,
                                                                     account_type account_type,
                                                                     sdk_process_type sdk_process_type,
                                                                     const database_pool_shared_ptr& mysql_pool) const
{
    if (password.empty())
    {
        if (account.get_password_hash().empty())
        {
            account.set_device_id(account.get_account_name());
            account.set_password_hash(generate_token());
        }
    }
    else
    {
        const auto salt = generate_token();
        const auto hashed_password = hmac_sha256::calculate(password, salt);
        account.set_salt(salt);
        account.set_password_hash(hashed_password);
    }

    const auto server_config = get_app_config()->get_server_config();
    const auto account_bind_id = snowflake_generator::get_instance().generate(server_config.get_datacenter_id(), server_config.get_worker_id());
    account_bind account_bind{ database_type::mysql, account_bind_id };
    account_bind.set_account_id(account.get_account_id());
    account_bind.set_auth_key(auth_key);
    account_bind.set_app_id(app_id);
    account_bind.set_account_type(static_cast<int>(account_type));
    account_bind.set_process_type(static_cast<int>(sdk_process_type));

    if (co_await mysql_pool->execute_changes(account.get_modify()) &&
        co_await mysql_pool->execute_changes(account_bind.get_modify()))
    {
        co_return true;
    }

    co_return false;
}