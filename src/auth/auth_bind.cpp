#include "auth_bind.h"
#include "common/snowflake_generator.h"
#include "config/app_config.h"
#include "database/database_pool_base.h"
#include "database/generated/mysql/auth/account_bind.h"

celeritas::auth_bind::auth_bind(http_handle_parameter handle_parameter)
    : base_type{ std::move(handle_parameter) }
{
}

celeritas::auth_bind::bool_awaitable_type celeritas::auth_bind::bind(account& account,
                                                                     const int64_t app_id,
                                                                     const std::string& phone,
                                                                     account_type account_type,
                                                                     const database_pool_shared_ptr& mysql_pool) const
{
    if (account.get_password_hash().empty())
    {
        account.set_device_id(account.get_account_name());
        account.set_password_hash(generate_token());
    }

    const auto server_config = get_app_config()->get_server_config();
    const auto account_bind_id = snowflake_generator::get_instance().generate(server_config.get_datacenter_id(), server_config.get_worker_id());
    account_bind account_bind{ database_type::mysql, account_bind_id };
    account_bind.set_account_id(account.get_account_id());
    account_bind.set_auth_key(phone);
    account_bind.set_app_id(app_id);
    account_bind.set_account_type(static_cast<int>(account_type));

    if (co_await mysql_pool->execute_changes(account.get_modify()) &&
        co_await mysql_pool->execute_changes(account_bind.get_modify()))
    {
        co_return true;
    }
    else
    {
        co_return false;
    }
}