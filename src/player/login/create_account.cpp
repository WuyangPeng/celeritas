#include "create_account.h"
#include "database/pool/database_pool_manager.h"
#include "database/generated/mysql/auth/account.h"
#include "database/generated/mysql/auth/account_bind.h"

celeritas::create_account::create_account(protobuf_handle_parameter_shared_ptr protobuf_handle_parameter, const proto::service::service_login_request& login)
    : protobuf_handle_parameter_{ std::move(protobuf_handle_parameter) }, login_{ login }
{
}

celeritas::create_account::bool_awaitable_type celeritas::create_account::save_database() const
{
    const auto redis_pool = database_pool_manager::get_instance().get_pool(redis_db_name.data());

    const auto optional_account = co_await redis_pool->select_one(account::get_select(database_type::redis, login_.account_id()), account::get_database_field_container());
    if (!optional_account)
    {
        co_return false;
    }

    account account{ database_type::mysql, *optional_account };

    const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_auth_db_name.data());

    if (!co_await mysql_pool->execute_changes(account.get_modify()))
    {
        co_return false;
    }

    if (login_.account_bind_id() <= 0)
    {
        co_return true;
    }

    const auto optional_account_bind = co_await redis_pool->select_one(account_bind::get_select(database_type::redis, login_.account_bind_id()), account_bind::get_database_field_container());
    if (!optional_account_bind)
    {
        co_return false;
    }

    if (account_bind account_bind{ database_type::mysql, *optional_account_bind };
        !co_await mysql_pool->execute_changes(account_bind.get_modify()))
    {
        co_return false;
    }

    co_return true;
}