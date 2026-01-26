#include "password_bind.h"
#include "password_bind_response.h"
#include "auth/core/auth_bind.tpp"
#include "config/aggregate/app_config.h"
#include "database/database_constant.h"
#include "database/pool/database_pool_manager.h"
#include "detail/password_bind_parameter.h"
#include "../../initializer/server/account_type.h"
#include "message/basic/game_error_type.h"

celeritas::password_bind::password_bind(http_handle_parameter_shared_ptr handle_parameter)
    : base_type{ std::move(handle_parameter) }
{
}

celeritas::password_bind::void_awaitable_type celeritas::password_bind::response()
{
    password_bind_parameter password_bind_parameter{ get_http_handle_parameter() };
    if (password_bind_parameter.is_failure())
    {
        co_return co_await write_immediately(password_bind_parameter.get_response());
    }

    const auto app_id = password_bind_parameter.get_app_id();
    const auto token = password_bind_parameter.get_token();
    const auto auth_key = password_bind_parameter.get_account();
    const auto password = password_bind_parameter.get_password();

    const auto redis_pool = database_pool_manager::get_instance().get_pool(redis_db_name.data());
    const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_auth_db_name.data());

    auto optional_account = co_await get_account<password_bind_response>(app_id, auth_key, token, account_type::password, redis_pool, mysql_pool);

    if (auto account = *optional_account;
        co_await bind(account, app_id, auth_key, password, account_type::password, sdk_process_type::null, mysql_pool))
    {
        co_return co_await write_immediately(password_bind_response{ game_error_type::success, "password bind success" });
    }

    co_return co_await write_immediately(password_bind_response{ game_error_type::mysql_error });
}

celeritas::http_service_base::void_awaitable_type celeritas::password_bind::send_error_response()
{
    co_return co_await write_immediately(password_bind_response{ game_error_type::unknown });
}