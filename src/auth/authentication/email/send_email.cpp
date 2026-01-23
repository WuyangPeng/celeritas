#include "send_email.h"
#include "send_email_response.h"
#include "auth/auth_constant.h"
#include "auth/data/app_email_providers.h"
#include "auth/data/app_secret.h"
#include "common/core/random_helper.h"
#include "database/database_constant.h"
#include "database/pool/database_pool_manager.h"
#include "database/generated/redis/auth/email_code.h"
#include "database/generated/redis/auth/email_limit.h"
#include "detail/send_email_parameter.h"
#include "detail/send_email_to_providers.h"
#include "message/basic/game_error_type.h"

celeritas::send_email::send_email(http_handle_parameter_shared_ptr handle_parameter)
    : base_type{ std::move(handle_parameter) }
{
}

celeritas::send_email::void_awaitable_type celeritas::send_email::response()
{
    send_email_parameter send_email_parameter{ get_http_handle_parameter() };

    if (send_email_parameter.is_failure())
    {
        co_return co_await write_immediately(send_email_parameter.get_response());
    }

    const auto email = send_email_parameter.get_email();
    const auto app = send_email_parameter.get_apps();

    const auto redis_pool = database_pool_manager::get_instance().get_pool(redis_db_name.data());

    if (auto sms_limit = co_await redis_pool->select_one(email_limit::get_select(database_type::redis, email), email_limit::get_database_field_container()))
    {
        co_return co_await write_immediately(send_email_response{ game_error_type::sent_too_frequently });
    }

    email_code email_code{ database_type::redis, email };
    email_code.set_code(random_helper::get_random_int(email_limit_code_begin, email_limit_code_end));

    email_limit email_limit{ database_type::redis, email };
    email_limit.set_exist(true);

    if (co_await redis_pool->execute_changes(email_code.get_modify(), email_code_expiration_time) &&
        co_await redis_pool->execute_changes(email_limit.get_modify(), email_limit_expiration_time))
    {
        co_return co_await write_immediately(send_email_response{ game_error_type::success, "send email success" });

        boost::asio::co_spawn(get_any_io_executor(),
                              [ email_code,app] {
                                  return send_sdk_sms(email_code, app);
                              }, boost::asio::detached);
    }
    else
    {
        co_return co_await write_immediately(send_email_response{ game_error_type::redis_error });
    }

    co_return;
}

celeritas::auth_service_base::void_awaitable_type celeritas::send_email::send_sdk_sms(const email_code& sms_code, const apps& apps)
{
    const auto email_providers = app_email_providers::get_instance().get_email_providers(apps.get_sms_provider_id());

    const auto provider = send_email_to_providers::create(sms_code, email_providers);

    co_return co_await provider->execute();
}