#include "send_sms.h"
#include "send_sms_response.h"
#include "auth/auth_constant.h"
#include "auth/config/app_sms_providers.h"
#include "common/core/random_helper.h"
#include "database/database_constant.h"
#include "database/pool/database_pool_manager.h"
#include "database/generated/redis/auth/sms_code.h"
#include "database/generated/redis/auth/sms_limit.h"
#include "detail/send_sms_parameter.h"
#include "detail/send_sms_to_providers.h"
#include "message/basic/game_error_type.h"

celeritas::send_sms::send_sms(http_handle_parameter_shared_ptr handle_parameter)
    : base_type{ std::move(handle_parameter) }
{
}

celeritas::send_sms::void_awaitable_type celeritas::send_sms::response()
{
    send_sms_parameter send_sms_parameter{ get_http_handle_parameter() };

    if (send_sms_parameter.is_failure())
    {
        co_return co_await write_immediately(send_sms_parameter.get_response());
    }

    const auto phone = send_sms_parameter.get_phone();
    const auto app = send_sms_parameter.get_apps();

    const auto redis_pool = database_pool_manager::get_instance().get_pool(redis_db_name.data());

    if (auto sms_limit = co_await redis_pool->select_one(sms_limit::get_select(database_type::redis, phone), sms_limit::get_database_field_container()))
    {
        co_return co_await write_immediately(send_sms_response{ game_error_type::sent_too_frequently });
    }

    sms_code sms_code{ database_type::redis, phone };
    sms_code.set_code(random_helper::get_random_int(sms_limit_code_begin, sms_limit_code_end));

    sms_limit sms_limit{ database_type::redis, phone };
    sms_limit.set_exist(true);

    if (co_await redis_pool->execute_changes(sms_code.get_modify(), sms_code_expiration_time) &&
        co_await redis_pool->execute_changes(sms_limit.get_modify(), sms_limit_expiration_time))
    {
        co_return co_await write_immediately(send_sms_response{ game_error_type::success, "send sms success" });

        boost::asio::co_spawn(get_any_io_executor(),
                              [ sms_code,app] {
                                  return send_sdk_sms(sms_code, app);
                              }, boost::asio::detached);
    }

    co_return co_await write_immediately(send_sms_response{ game_error_type::redis_error });
}

celeritas::http_service_base::void_awaitable_type celeritas::send_sms::send_error_response()
{
    co_return co_await write_immediately(send_sms_response{ game_error_type::unknown, "unknown error" });
}

celeritas::send_sms::void_awaitable_type celeritas::send_sms::send_sdk_sms(const sms_code& sms_code, const apps& apps)
{
    const auto sms_providers = app_sms_providers::get_instance().get_sms_providers(apps.get_sms_provider_id());

    const auto provider = send_sms_to_providers::create(sms_code, *sms_providers);

    co_return co_await provider->execute();
}