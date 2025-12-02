#pragma once

#include <string_view>

namespace celeritas
{
    class payment_parameter;
    class order_create_parameter;
    class payment_params_json;

    constexpr std::string_view timestamp_describe = "timestamp";
    constexpr std::string_view sign_describe = "sign";
    constexpr std::string_view client_request_id_describe = "client_request_id";
    constexpr std::string_view account_id_describe = "account_id";
    constexpr std::string_view game_server_id_describe = "game_server_id";
    constexpr std::string_view role_id_describe = "role_id";
    constexpr std::string_view product_id_describe = "product_id";
    constexpr std::string_view amount_describe = "amount";
    constexpr std::string_view platform_describe = "platform";
}