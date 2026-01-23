#pragma once

#include "phone_parameter.h"
#include "auth/auth_fwd.h"
#include "database/generated/redis/auth/sms_code.h"

namespace celeritas
{
    class phone_operation_parameter : public phone_parameter
    {
    public:
        using class_type = phone_operation_parameter;
        using base_type = phone_parameter;
        using optional_sms_code = std::optional<sms_code>;
        using optional_sms_code_awaitable_type = boost::asio::awaitable<optional_sms_code>;
        using database_pool_shared_ptr = std::shared_ptr<database_pool_base>;

        explicit phone_operation_parameter(const http_handle_parameter_shared_ptr& http_handle_parameter);

        [[nodiscard]] int get_code() const;

        [[nodiscard]] optional_http_response get_http_parameter() override;

        template <typename ResponseType, typename ServiceBaseType>
        [[nodiscard]] optional_sms_code_awaitable_type check_code(const database_pool_shared_ptr& redis_pool,
                                                                  ServiceBaseType& auth_service_base) const;

    private:
        int code_;
    };
}