#pragma once

#include "email_parameter.h"
#include "auth/auth_fwd.h"
#include "database/generated/redis/auth/email_code.h"

namespace celeritas
{
    class email_operation_parameter : public email_parameter
    {
    public:
        using class_type = email_operation_parameter;
        using base_type = email_parameter;
        using optional_email_code = std::optional<email_code>;
        using optional_email_code_awaitable_type = boost::asio::awaitable<optional_email_code>;
        using database_pool_shared_ptr = std::shared_ptr<database_pool_base>;

        explicit email_operation_parameter(const http_handle_parameter_shared_ptr& http_handle_parameter);

        [[nodiscard]] int get_code() const;

        [[nodiscard]] optional_http_response get_http_parameter() override;

        template <typename ResponseType>
        [[nodiscard]] optional_email_code_awaitable_type check_code(const database_pool_shared_ptr& redis_pool,
                                                                    auth_service_base& auth_service_base) const;

    private:
        int code_;
    };
}