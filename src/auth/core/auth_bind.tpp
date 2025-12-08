#pragma once

#include "auth_bind.h"
#include "auth/authentication/phone/phone_bind_response.h"
#include "auth/authentication/sdk/sdk_process_type.h"
#include "database/database_pool_base.h"
#include "database/generated/mysql/auth/account_bind.h"
#include "database/generated/redis/auth/session_token.h"
#include "initializer/account_type.h"

template <typename ResponseType>
celeritas::auth_bind::optional_account_awaitable_type celeritas::auth_bind::get_account(int64_t app_id,
                                                                                        const std::string& auth_key,
                                                                                        const std::string& token,
                                                                                        account_type account_type,
                                                                                        const database_pool_shared_ptr& redis_pool,
                                                                                        const database_pool_shared_ptr& mysql_pool) const
{
    const auto optional_session_token = co_await redis_pool->select_one(session_token::get_select(database_type::redis, token), session_token::get_database_field_container());
    if (!optional_session_token)
    {
        write(ResponseType{ game_error_type::token_error });
    }

    const session_token session_token{ *optional_session_token };

    const auto optional_account = co_await mysql_pool->select_one(account::get_select(database_type::mysql, session_token.get_account_id()),
                                                                  account::get_database_field_container());

    if (!optional_account)
    {
        write(ResponseType{ game_error_type::account_error });
    }
    const auto key = std::make_shared<basis_database_container>(basis_database_container::object_container{ { account_bind::account_type_describe, static_cast<int>(account_type) },
                                                                                                            { account_bind::process_type_describe, static_cast<int>(sdk_process_type::null) },
                                                                                                            { account_bind::auth_key_describe, auth_key },
                                                                                                            { account_bind::app_id_describe, app_id }
    });

    if (auto optional_account_bind = co_await mysql_pool->select_one(account_bind::get_select(database_type::mysql, key), account::get_database_field_container()))
    {
        write(ResponseType{ game_error_type::account_bound });

        co_return std::nullopt;
    }

    co_return account{ *optional_account };
}