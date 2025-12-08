#pragma once

#include "auth/server_list/login_servers_response.h"
#include "auth_parameter.h"
#include "database/generated/mysql/auth/apps.h"

namespace celeritas {
    class login_servers_parameter final : public auth_parameter {
    public:
        using class_type = login_servers_parameter;
        using base_type = auth_parameter;

        explicit login_servers_parameter(const http_handle_parameter &http_handle_parameter);

        [[nodiscard]] bool is_failure() const override;

        [[nodiscard]] login_servers_response get_response() const;

        [[nodiscard]] std::string get_token() const;

        [[nodiscard]] optional_string get_zone() const;

        [[nodiscard]] bool is_only_preferred() const;

        [[nodiscard]] bool is_include_details() const;

        [[nodiscard]] bool is_websocket() const;

        [[nodiscard]] apps get_apps() const;

    private:
        using optional_apps = std::optional<apps>;
        using optional_login_servers_response = std::optional<login_servers_response>;

        void init();

        [[nodiscard]] std::string get_actual_zone() const;

        optional_login_servers_response response_;
        std::string token_;
        optional_string zone_;
        bool only_preferred_;
        bool include_details_;
        bool websocket_;
        optional_apps apps_;
    };
}