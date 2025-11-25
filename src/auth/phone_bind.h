#pragma once

#include "database/database_pool.h"
#include "database/generated/mysql/account.h"
#include "message/http_handle_parameter.h"

#include <boost/asio/awaitable.hpp>

namespace celeritas
{
    class phone_bind
    {
    public:
        using class_type = phone_bind;
        using void_awaitable_type = boost::asio::awaitable<void>;

        explicit phone_bind(http_handle_parameter handle_parameter);

        [[nodiscard]] void_awaitable_type response();

    private:
        using account_awaitable_type = boost::asio::awaitable<account>;
        using result_container = database_pool::result_container;
        using database_pool_shared_ptr = std::shared_ptr<database_pool>;
        using const_app_config_shared_ptr = std::shared_ptr<const app_config>;
        using optional_basis_database_manager = std::optional<basis_database_manager>;

        [[nodiscard]] static std::string calculate_hmac_sha256(int app_id, const std::string& phone, const std::string& device_id, int code, int64_t timestamp, const std::string& secret_key);

        [[nodiscard]] std::string generate_token();

        http_handle_parameter handle_parameter_;
    };
}