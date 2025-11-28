#pragma once

#include <string_view>

namespace celeritas
{
    class auth_parameter;
    class guest_login_parameter;

    constexpr std::string_view timestamp_describe = "timestamp";
    constexpr std::string_view sign_describe = "sign";
}