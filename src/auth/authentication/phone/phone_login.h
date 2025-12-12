#pragma once

#include "auth/core/auth_login.h"
#include "database/database_pool_base.h"
#include "database/generated/mysql/auth/account.h"
#include "database/generated/redis/auth/sms_code.h"
#include "message/http_handle_parameter.h"

namespace celeritas
{
    class phone_login final : public auth_login
    {
    public:
        using class_type = phone_login;
        using base_type = auth_login;

        explicit phone_login(http_handle_parameter_shared_ptr handle_parameter);

        [[nodiscard]] void_awaitable_type response() override;

    private:
        using account_awaitable_type = boost::asio::awaitable<account>;
        using database_pool_shared_ptr = std::shared_ptr<database_pool_base>;
        using const_app_config_shared_ptr = std::shared_ptr<const app_config>;
        using optional_database_entity_change = std::optional<database_entity_change>;

        [[nodiscard]] account_awaitable_type get_account(const optional_database_entity_change& database_entity_change,
                                                         const database_pool_shared_ptr& redis_pool,
                                                         const database_pool_shared_ptr& mysql_pool,
                                                         int64_t app_id,
                                                         const std::string& phone,
                                                         const const_app_config_shared_ptr& app_config);
    };
}