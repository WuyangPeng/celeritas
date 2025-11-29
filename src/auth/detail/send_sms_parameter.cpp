#include "send_sms_parameter.h"
#include "auth/app_secret.h"
#include "common/hmac_sha_256.h"

#include <regex>

celeritas::send_sms_parameter::send_sms_parameter(const http_handle_parameter& http_handle_parameter)
    : base_type{ std::move(http_handle_parameter) }, response_{}, phone_{}, apps_{}
{
    init();
}

std::string celeritas::send_sms_parameter::get_phone() const
{
    return phone_;
}

bool celeritas::send_sms_parameter::is_failure() const
{
    return response_.has_value();
}

celeritas::send_sms_response celeritas::send_sms_parameter::get_response() const
{
    return response_.value();
}

celeritas::apps celeritas::send_sms_parameter::get_apps() const
{
    return *apps_;
}

void celeritas::send_sms_parameter::init()
{
    const auto optional_phone = get_param("phone");
    if (!optional_phone)
    {
        response_ = send_sms_response{ game_error_type::invalid_parameter, "phone is required" };

        return;
    }

    const auto& phone = *optional_phone;
    if (const std::regex phone_regex{ R"(^1\d{10}$)" };
        !std::regex_match(phone, phone_regex))
    {
        response_ = send_sms_response{ game_error_type::invalid_parameter, "phone is invalid" };

        return;
    }

    phone_ = *optional_phone;

    if (const auto http_response = get_http_parameter())
    {
        response_ = send_sms_response{ *http_response };
        return;
    }

    apps_ = app_secret::get_instance().get_apps(get_app_id());

    if (const auto hmac_sha256 = hmac_sha256::calculate_with_args((*apps_).get_app_secret(), get_app_id(), phone_, get_timestamp());
        hmac_sha256 != get_sign())
    {
        response_ = send_sms_response{ game_error_type::sign_error };
    }
}