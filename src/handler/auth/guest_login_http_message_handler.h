#pragma once

#include "database/database_pool_base.h"
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

        [[nodiscard]] bool handle(const http_handle_parameter& handle_parameter,
                                  const http_message_registry_weak_ptr& message_registry) override;

    private:
        using void_awaitable_type = boost::asio::awaitable<void>;
        using result_container = database_pool_base::result_container;
        using account_awaitable_type = boost::asio::awaitable<account>;
        using database_pool_shared_ptr = std::shared_ptr<database_pool_base>;
        using const_app_config_shared_ptr = std::shared_ptr<const app_config>;
        using optional_database_entity_change = std::optional<database_entity_change>;

        [[nodiscard]] void_awaitable_type response(http_handle_parameter handle_parameter);
    };
}