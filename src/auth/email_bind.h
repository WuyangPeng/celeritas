#pragma once

#include "database/database_pool_base.h"
#include "database/generated/mysql/auth/account.h"
#include "message/http_handle_parameter.h"

#include <boost/asio/awaitable.hpp>

namespace celeritas
{
    class email_bind
    {
    public:
        using class_type = email_bind;
        using void_awaitable_type = boost::asio::awaitable<void>;

        explicit email_bind(http_handle_parameter handle_parameter);

        [[nodiscard]] void_awaitable_type response();

    private:
        using account_awaitable_type = boost::asio::awaitable<account>;
        using result_container = database_pool_base::result_container;
        using database_pool_shared_ptr = std::shared_ptr<database_pool_base>;
        using const_app_config_shared_ptr = std::shared_ptr<const app_config>;
        using optional_database_entity_change = std::optional<database_entity_change>;

        [[nodiscard]] static std::string calculate_hmac_sha256(int64_t app_id, const std::string& email, const std::string& token, int code, int64_t timestamp, const std::string& secret_key);

        [[nodiscard]] std::string generate_token();

        http_handle_parameter handle_parameter_;
    };
}