#pragma once

#include "login_server_info.h"
#include "message/http_response.h"

namespace celeritas
{
    class login_servers_response final : public http_response
    {
    public:
        using class_type = login_servers_response;
        using bass_type = http_response;
        using container = std::vector<login_server_info>;

        login_servers_response() noexcept = default;

        explicit login_servers_response(game_error_type code);

        login_servers_response(game_error_type code, std::string message);

        login_servers_response(game_error_type code, std::string message, container login_server_info);

        login_servers_response(game_error_type code, std::string message, login_server_info login_server_info);

        explicit login_servers_response(bass_type http_response);

        login_servers_response(bass_type http_response, container login_server_info);

        [[nodiscard]] container get_login_server_info() const;

        [[nodiscard]] std::string to_json_string() const override;

        [[nodiscard]] static login_servers_response from_json_string(const std::string& json_string);

        [[nodiscard]] static login_servers_response tag_invoke(const json_value& value);

        static constexpr std::string_view login_server_info_description = "login_server_info";

    private:
        container login_server_info_;
    };

    using login_servers_response_tag = boost::json::value_to_tag<login_servers_response>;

    [[nodiscard]] login_servers_response tag_invoke(login_servers_response_tag, const login_servers_response::json_value& value);

    void tag_invoke(boost::json::value_from_tag, login_servers_response::json_value& value, const login_servers_response& login_servers_response);
}

