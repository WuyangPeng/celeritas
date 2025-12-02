#include "order_create_parameter.h"
#include "payment_internal_fwd.h"
#include "auth/app_secret.h"
#include "common/hmac_sha_256.h"

#include <boost/lexical_cast.hpp>

celeritas::order_create_parameter::order_create_parameter(const http_handle_parameter& http_handle_parameter)
    : base_type{ http_handle_parameter }, client_request_id_{}, account_id_{}, game_server_id_{}, role_id_{}, product_id_{}, amount_{}, platform_{}, apps_{}
{
}

bool celeritas::order_create_parameter::is_failure() const
{
    return response_.has_value();
}

celeritas::order_create_http_response celeritas::order_create_parameter::get_response() const
{
    return *response_;
}

celeritas::apps celeritas::order_create_parameter::get_apps() const
{
    return *apps_;
}

void celeritas::order_create_parameter::init()
{
    const auto optional_client_request_id = get_param(client_request_id_describe.data());
    if (!optional_client_request_id)
    {
        response_ = order_create_http_response{ game_error_type::invalid_parameter, "client request id is required" };

        return;
    }

    const auto optional_account_id = get_param(account_id_describe.data());
    if (!optional_account_id)
    {
        response_ = order_create_http_response{ game_error_type::invalid_parameter, "account id is required" };

        return;
    }

    const auto optional_game_server_id = get_param(game_server_id_describe.data());
    if (!optional_game_server_id)
    {
        response_ = order_create_http_response{ game_error_type::invalid_parameter, "game server id is required" };

        return;
    }

    const auto optional_role_id = get_param(role_id_describe.data());
    if (!optional_role_id)
    {
        response_ = order_create_http_response{ game_error_type::invalid_parameter, "role id is required" };

        return;
    }

    const auto optional_product_id = get_param(product_id_describe.data());
    if (!optional_product_id)
    {
        response_ = order_create_http_response{ game_error_type::invalid_parameter, "product id is required" };

        return;
    }

    const auto optional_amount = get_param(amount_describe.data());
    if (!optional_amount)
    {
        response_ = order_create_http_response{ game_error_type::invalid_parameter, "amount is required" };

        return;
    }

    const auto optional_platform = get_param(platform_describe.data());
    if (!optional_platform)
    {
        response_ = order_create_http_response{ game_error_type::invalid_parameter, "platform is required" };

        return;
    }

    client_request_id_ = *optional_client_request_id;
    account_id_ = boost::lexical_cast<int64_t>(*optional_account_id);
    game_server_id_ = *optional_game_server_id;
    role_id_ = boost::lexical_cast<int64_t>(*optional_role_id);
    product_id_ = *optional_product_id;
    amount_ = boost::lexical_cast<int>(*optional_amount);
    platform_ = *optional_platform;
    apps_ = app_secret::get_instance().get_apps(get_app_id());

    if (const auto http_response = get_http_parameter())
    {
        response_ = order_create_http_response{ *http_response };
    }

    if (const auto hmac_sha256 = hmac_sha256::calculate_with_args(apps_->get_app_payment_secret(), client_request_id_, account_id_, game_server_id_, role_id_, product_id_, amount_, platform_, get_timestamp());
        hmac_sha256 != get_sign())
    {
        response_ = order_create_http_response{ game_error_type::sign_error };
    }
}