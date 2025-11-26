#pragma once

#include "database/database_pool.h"
#include "database/generated/mysql/auth/account.h"
#include "message/http_handle_parameter.h"

#include <boost/asio/awaitable.hpp>

namespace celeritas
{
    class email_login
    {
    public:
        using class_type = email_login;
        using void_awaitable_type = boost::asio::awaitable<void>;

        explicit email_login(http_handle_parameter handle_parameter);

        [[nodiscard]] void_awaitable_type response();

    private:
        using account_awaitable_type = boost::asio::awaitable<account>;
        using result_container = database_pool::result_container;
        using database_pool_shared_ptr = std::shared_ptr<database_pool>;
        using const_app_config_shared_ptr = std::shared_ptr<const app_config>;
        using optional_basis_database_manager = std::optional<basis_database_manager>;

        [[nodiscard]] static std::string calculate_hmac_sha256(int app_id, const std::string& email, int code, int64_t timestamp, const std::string& secret_key);

        [[nodiscard]] std::string generate_token();

        [[nodiscard]] account_awaitable_type get_account(const optional_basis_database_manager& basis_database_manager,
                                                         const database_pool_shared_ptr& redis_pool,
                                                         const database_pool_shared_ptr& mysql_pool,
                                                         const std::string& email,
                                                         const const_app_config_shared_ptr& app_config);

        http_handle_parameter handle_parameter_;
    };
}