#pragma once

#include <string_view>

namespace celeritas
{
    constexpr auto sms_code_expiration_time = 300;
    constexpr auto sms_limit_expiration_time = 60;
    constexpr auto email_code_expiration_time = 900;
    constexpr auto email_limit_expiration_time = 60;
    constexpr auto sms_limit_code_begin = 100000;
    constexpr auto sms_limit_code_end = 999999;
    constexpr auto email_limit_code_begin = 100000;
    constexpr auto email_limit_code_end = 999999;
    constexpr auto sms_code_retry_count = 5;
    constexpr auto email_code_retry_count = 5;

    constexpr std::string_view timestamp_describe = "timestamp";
    constexpr std::string_view sign_describe = "sign";
    constexpr std::string_view phone_describe = "phone";
    constexpr std::string_view email_describe = "email";
    constexpr std::string_view account_describe = "account";
    constexpr std::string_view password_describe = "password";
    constexpr std::string_view code_describe = "code";
    constexpr std::string_view token_describe = "token";
    constexpr std::string_view sdk_token_describe = "sdk_token";
    constexpr std::string_view type_describe = "type";
    constexpr std::string_view zone_describe = "zone";
    constexpr std::string_view only_preferred_describe = "only_preferred";
    constexpr std::string_view include_details_describe = "include_details";
    constexpr std::string_view is_websocket_describe = "is_websocket";
}