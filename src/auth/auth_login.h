#pragma once

#include "auth_service_base.h"
#include "database/generated/mysql/auth/account.h"
#include "database/generated/redis/auth/session_token.h"
#include "server/server_fwd.h"

namespace celeritas
{
    class auth_login : public auth_service_base
    {
    public:
        using class_type = auth_login;
        using base_type = auth_service_base;

        explicit auth_login(http_handle_parameter handle_parameter);

    protected:
        using account_awaitable_type = boost::asio::awaitable<account>;
        using optional_database_entity_change = std::optional<database_entity_change>;
        using optional_database_entity_change_awaitable_type = boost::asio::awaitable<optional_database_entity_change>;
        using database_pool_shared_ptr = std::shared_ptr<database_pool_base>;
        using const_app_config_shared_ptr = std::shared_ptr<const app_config>;
        using optional_session_token = std::optional<session_token>;
        using session_token_awaitable_type = boost::asio::awaitable<optional_session_token>;

        [[nodiscard]] static optional_database_entity_change_awaitable_type get_optional_account(int64_t app_id,
                                                                                                 const std::string& device_id,
                                                                                                 const database_pool_shared_ptr& mysql_pool);

        [[nodiscard]] static account_awaitable_type create_new_account(int64_t app_id,
                                                                       const std::string& device_id,
                                                                       const database_pool_shared_ptr& redis_pool,
                                                                       const const_app_config_shared_ptr& app_config);

        [[nodiscard]] static session_token_awaitable_type create_session_token(const account& account,
                                                                               bool is_new_account,
                                                                               const database_pool_shared_ptr& redis_pool);

        [[nodiscard]] static account_awaitable_type create_new_account(int64_t app_id,
                                                                       const std::string& auth_key,
                                                                       account_type account_type,
                                                                       const std::string& account_name_prefix,
                                                                       const database_pool_shared_ptr& redis_pool,
                                                                       const const_app_config_shared_ptr& app_config);

        [[nodiscard]] static account_awaitable_type create_new_account(int64_t app_id,
                                                                       const std::string& auth_key,
                                                                       const std::string& password,
                                                                       account_type account_type,
                                                                       const std::string& account_name_prefix,
                                                                       const database_pool_shared_ptr& redis_pool,
                                                                       const const_app_config_shared_ptr& app_config);
    };
}