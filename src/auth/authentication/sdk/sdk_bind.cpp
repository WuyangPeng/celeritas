#include "sdk_bind.h"
#include "sdk_bind_response.h"
#include "auth/core/auth_bind.tpp"
#include "auth/data/app_sdk_providers.h"
#include "auth/detail/sdk/sdk_bind_parameter.h"
#include "auth/detail/sdk/sdk_process.h"
#include "auth/detail/sdk/sdk_process_parameter.h"
#include "database/pool/database_pool_manager.h"
#include "database/generated/mysql/auth/account_bind.h"
#include "initializer/account_type.h"
#include "message/basic/game_error_type.h"

celeritas::sdk_bind::sdk_bind(http_handle_parameter_shared_ptr handle_parameter)
    : base_type{ std::move(handle_parameter) }
{
}

celeritas::sdk_bind::void_awaitable_type celeritas::sdk_bind::response()
{
    sdk_bind_parameter sdk_bind_parameter{ get_http_handle_parameter() };

    if (sdk_bind_parameter.is_failure())
    {
        co_return co_await write_immediately(sdk_bind_parameter.get_response());
    }

    const auto app_id = sdk_bind_parameter.get_app_id();
    const auto sdk_token = sdk_bind_parameter.get_sdk_token();
    const auto process_type = sdk_bind_parameter.get_process_type();

    const auto sdk_providers = app_sdk_providers::get_instance().get_sdk_providers(sdk_providers_key{ app_id, process_type });

    sdk_process_parameter sdk_process_parameter{ sdk_token, sdk_providers };

    const auto sdk_process = sdk_process::create_sdk_process(sdk_process_parameter);

    const auto optional_open_id = co_await sdk_process->get_open_id();
    if (!optional_open_id)
    {
        co_return co_await write_immediately(sdk_bind_response{ game_error_type::sdk_error });
    }

    const auto& open_id = *optional_open_id;
    const auto token = sdk_bind_parameter.get_token();

    const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_auth_db_name.data());
    const auto redis_pool = database_pool_manager::get_instance().get_pool(redis_db_name.data());

    auto optional_account = co_await get_account<sdk_bind_response>(app_id, open_id, token, account_type::sdk, redis_pool, mysql_pool);

    if (auto account = *optional_account;
        co_await bind(account, app_id, open_id, account_type::sdk, process_type, mysql_pool))
    {
        co_return co_await write_immediately(sdk_bind_response{ game_error_type::success, "sdk bind success" });
    }
    else
    {
        co_return co_await write_immediately(sdk_bind_response{ game_error_type::mysql_error });
    }
}