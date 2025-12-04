#pragma once

#include "auth_service_base.h"
#include "database/document/server_role.h"
#include "database/generated/mongo/auth/player_server_roles.h"

#include <map>

namespace celeritas
{
    class login_servers final : public auth_service_base
    {
    public:
        using class_type = login_servers;
        using base_type = auth_service_base;

        explicit login_servers(http_handle_parameter handle_parameter);

        [[nodiscard]] void_awaitable_type response() override;

    private:
        using container = std::map<std::string, server_role>;

        container server_role_;
    };
}