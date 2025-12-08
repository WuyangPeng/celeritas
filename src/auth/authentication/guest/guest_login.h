#pragma once

#include "auth/core/auth_login.h"
#include "database/database_pool_base.h"
#include "database/generated/mysql/auth/account.h"

namespace celeritas
{
    class guest_login final : public auth_login
    {
    public:
        using class_type = guest_login;
        using base_type = auth_login;

        explicit guest_login(http_handle_parameter handle_parameter);

        [[nodiscard]] void_awaitable_type response() override;

    private:
        using account_awaitable_type = boost::asio::awaitable<account>;
        using database_pool_shared_ptr = std::shared_ptr<database_pool_base>;
        using const_app_config_shared_ptr = std::shared_ptr<const app_config>;
        using optional_database_entity_change = std::optional<database_entity_change>;

        [[nodiscard]] static account_awaitable_type get_account(int64_t app_id,
                                                                const std::string& device_id,
                                                                const optional_database_entity_change&
                                                                database_entity_change,
                                                                const database_pool_shared_ptr& redis_pool,
                                                                const const_app_config_shared_ptr& app_config);
    };
}