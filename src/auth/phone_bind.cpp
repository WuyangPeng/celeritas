#include "phone_bind.h"
#include "phone_bind_response.h"
#include "common/logger.h"
#include "common/snowflake_generator.h"
#include "config/app_config.h"
#include "database/database_pool_manager.h"
#include "database/generated/mysql/auth/account_bind.h"
#include "database/generated/redis/auth/session_token.h"
#include "detail/phone_bind_parameter.h"
#include "server/account_type.h"
#include "message/game_error_type.h"

celeritas::phone_bind::phone_bind(http_handle_parameter handle_parameter)
    : base_type{ std::move(handle_parameter) }
{
}

celeritas::phone_bind::void_awaitable_type celeritas::phone_bind::response()
{
    phone_bind_parameter phone_bind_parameter{ get_http_handle_parameter() };

    if (phone_bind_parameter.is_failure())
    {
        co_return write(phone_bind_parameter.get_response());
    }

    const auto redis_pool = database_pool_manager::get_instance().get_pool(redis_db_name.data());
    const auto optional_sms_code = co_await phone_bind_parameter.check_code(redis_pool, *this);
    if (!optional_sms_code)
    {
        co_return;
    }

    const auto app_id = phone_bind_parameter.get_app_id();
    const auto phone = phone_bind_parameter.get_phone();
    const auto token = phone_bind_parameter.get_token();

    const auto optional_session_token = co_await redis_pool->select_one(session_token::get_select(database_type::redis, token), session_token::get_database_field_container());
    if (!optional_session_token)
    {
        co_return write(phone_bind_response{ game_error_type::token_error });
    }

    session_token session_token{ *optional_session_token };

    const auto mysql_pool = database_pool_manager::get_instance().get_pool(auth_db_name.data());

    auto optional_account = co_await mysql_pool->select_one(account::get_select(database_type::mysql, session_token.get_account_id()), account::get_database_field_container());

    if (!optional_account)
    {
        co_return write(phone_bind_response{ game_error_type::account_error });
    }
    const auto key = std::make_shared<basis_database_container>(basis_database_container::object_container{ { account_bind::account_type_describe, static_cast<int>(account_type::phone) },
                                                                                                            { account_bind::auth_key_describe, phone },
                                                                                                            { account_bind::app_id_describe, app_id } });

    if (auto optional_account_bind = co_await mysql_pool->select_one(account_bind::get_select(database_type::mysql, key), account::get_database_field_container()))
    {
        co_return write(phone_bind_response{ game_error_type::account_bound });
    }

    account account{ *optional_account };
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
    account_bind.set_account_type(static_cast<int>(account_type::phone));

    if (co_await mysql_pool->execute_changes(account.get_modify()) &&
        co_await mysql_pool->execute_changes(account_bind.get_modify()))
    {
        write(phone_bind_response{ game_error_type::success, "phone bind success" });

        if (!co_await redis_pool->execute_changes(optional_sms_code->get_delete()))
        {
            LOG_CHANNEL(auth_channel, error) << "delete sms code error.";
        }
    }
    else
    {
        write(phone_bind_response{ game_error_type::mysql_error });
    }
}