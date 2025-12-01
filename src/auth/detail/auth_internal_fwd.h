#pragma once

#include <string_view>

namespace celeritas
{
    class auth_parameter;
    class guest_login_parameter;

    class phone_parameter;
    class send_sms_parameter;
    class phone_operation_parameter;
    class phone_login_parameter;
    class phone_bind_parameter;

    class email_parameter;
    class send_email_parameter;
    class email_operation_parameter;

    class send_sms_to_providers;
    class tencent_cloud_sms;
    class send_email_to_providers;
    class tencent_cloud_email;

    enum class send_sms_type;
    enum class send_email_type;

    constexpr std::string_view timestamp_describe = "timestamp";
    constexpr std::string_view sign_describe = "sign";
    constexpr std::string_view phone_describe = "phone";
    constexpr std::string_view email_describe = "email";
    constexpr std::string_view code_describe = "code";
    constexpr std::string_view token_describe = "token";
}