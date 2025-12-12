#pragma once

#include "auth/core/auth_service_base.h"
#include "auth/data/server_cell_repository.h"
#include "auth/detail/login_servers/login_servers_parameter.h"
#include "database/document/server_role.h"
#include "database/generated/redis/auth/session_token.h"

#include <map>

namespace celeritas
{
    class login_servers final : public auth_service_base
    {
    public:
        using class_type = login_servers;
        using base_type = auth_service_base;

        explicit login_servers(http_handle_parameter_shared_ptr handle_parameter);

        [[nodiscard]] void_awaitable_type response() override;

    private:
        using container = std::map<std::string, server_role>;
        using database_pool_shared_ptr = std::shared_ptr<database_pool_base>;

        [[nodiscard]] login_server_info get_login_server_info(const login_servers_parameter& login_servers_parameter,
                                                              const server_cell& server_cell);

        [[nodiscard]] void_awaitable_type create_server_role(const login_servers_parameter& login_servers_parameter,
                                                             const session_token& session_token);

        [[nodiscard]] void_awaitable_type response_is_only_preferred(const login_servers_parameter& login_servers_parameter,
                                                                     const session_token& session_token,
                                                                     const database_pool_shared_ptr& redis_pool);

        [[nodiscard]] void_awaitable_type response_is_all(const login_servers_parameter& login_servers_parameter);

        container server_role_;
    };
}