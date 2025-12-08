#pragma once

#include "auth_service_base.h"
#include "auth/auth_fwd.h"
#include "database/generated/mysql/auth/account.h"
#include "initializer/account_type.h"

namespace celeritas
{
    class auth_bind : public auth_service_base
    {
    public:
        using class_type = auth_bind;
        using base_type = auth_service_base;

        explicit auth_bind(http_handle_parameter handle_parameter);

    protected:
        using optional_account = std::optional<account>;
        using optional_account_awaitable_type = boost::asio::awaitable<optional_account>;
        using database_pool_shared_ptr = std::shared_ptr<database_pool_base>;
        using bool_awaitable_type = boost::asio::awaitable<bool>;

        template <typename ResponseType>
        [[nodiscard]] optional_account_awaitable_type get_account(int64_t app_id,
                                                                  const std::string& auth_key,
                                                                  const std::string& token,
                                                                  account_type account_type,
                                                                  const database_pool_shared_ptr& redis_pool,
                                                                  const database_pool_shared_ptr& mysql_pool) const;

        [[nodiscard]] bool_awaitable_type bind(account& account,
                                               int64_t app_id,
                                               const std::string& auth_key,
                                               account_type account_type,
                                               sdk_process_type sdk_process_type,
                                               const database_pool_shared_ptr& mysql_pool) const;

        [[nodiscard]] bool_awaitable_type bind(account& account,
                                               int64_t app_id,
                                               const std::string& auth_key,
                                               const std::string& password,
                                               account_type account_type,
                                               sdk_process_type sdk_process_type,
                                               const database_pool_shared_ptr& mysql_pool) const;
    };
}