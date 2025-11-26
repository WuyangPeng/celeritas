#pragma once

#include "database/database_pool.h"
#include "database/generated/mysql/auth/account.h"
#include "message/http_base_message_handler.h"

#include <boost/asio/awaitable.hpp>

namespace celeritas
{
    class guest_login_http_message_handler final : public http_base_message_handler
    {
    public:
        using class_type = guest_login_http_message_handler;
        using base_type = http_base_message_handler;

        guest_login_http_message_handler() noexcept = default;

        [[nodiscard]] std::string get_supported_type_name() const override;

        [[nodiscard]] bool handle(const http_handle_parameter& handle_parameter, const http_message_registry_weak_ptr& message_registry) override;

    private:
        using void_awaitable_type = boost::asio::awaitable<void>;
        using result_container = database_pool::result_container;
        using account_awaitable_type = boost::asio::awaitable<account>;
        using database_pool_shared_ptr = std::shared_ptr<database_pool>;
        using const_app_config_shared_ptr = std::shared_ptr<const app_config>;
        using optional_basis_database_manager = std::optional<basis_database_manager>;

        [[nodiscard]] void_awaitable_type guest_login(http_handle_parameter handle_parameter);

        [[nodiscard]] void_awaitable_type do_guest_login(http_handle_parameter handle_parameter);

        [[nodiscard]] account_awaitable_type get_account(const optional_basis_database_manager& basis_database_manager, const database_pool_shared_ptr& database_pool, const std::string& device_id, const const_app_config_shared_ptr& app_config);

        [[nodiscard]] static std::string generate_token();

        [[nodiscard]] static std::string calculate_hmac_sha256(int app_id, const std::string& device_id, int64_t timestamp, const std::string& secret_key);
    };
}
