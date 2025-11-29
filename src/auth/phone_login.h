#pragma once

#include "auth_login.h"
#include "database/database_pool_base.h"
#include "database/generated/mysql/auth/account.h"
#include "database/generated/redis/auth/sms_code.h"
#include "message/http_handle_parameter.h"

#include <boost/asio/awaitable.hpp>

namespace celeritas
{
    class phone_login final : public auth_login
    {
    public:
        using class_type = phone_login;
        using base_type = auth_login;

        using void_awaitable_type = boost::asio::awaitable<void>;

        explicit phone_login(http_handle_parameter handle_parameter);

        [[nodiscard]] void_awaitable_type response() override;

    private:
        using account_awaitable_type = boost::asio::awaitable<account>;
        using result_container = database_pool_base::result_container;
        using database_pool_shared_ptr = std::shared_ptr<database_pool_base>;
        using const_app_config_shared_ptr = std::shared_ptr<const app_config>;
        using optional_basis_database_manager = std::optional<database_entity_change>;
        using optional_sms_code = std::optional<sms_code>;
        using optional_sms_code_awaitable_type = boost::asio::awaitable<optional_sms_code>;

        [[nodiscard]] static account_awaitable_type get_account(const optional_basis_database_manager& basis_database_manager,
                                                                const database_pool_shared_ptr& redis_pool,
                                                                const database_pool_shared_ptr& mysql_pool,
                                                                int64_t app_id,
                                                                const std::string& phone,
                                                                const const_app_config_shared_ptr& app_config);

        [[nodiscard]] optional_sms_code_awaitable_type check_code(int code,
                                                                  const std::string& phone,
                                                                  const database_pool_shared_ptr& redis_pool) const;
    };
}