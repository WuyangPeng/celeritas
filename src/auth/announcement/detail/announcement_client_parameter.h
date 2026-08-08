#pragma once

#include "auth/core/detail/auth_parameter.h"
#include "message/basic/http_response.h"

namespace celeritas
{
    class announcement_client_parameter final : public auth_parameter
    {
    public:
        using class_type = announcement_client_parameter;
        using base_type = auth_parameter;

        explicit announcement_client_parameter(const http_handle_parameter_shared_ptr& http_handle_parameter);

        [[nodiscard]] bool is_failure() const override;

        [[nodiscard]] http_response get_response() const;

    private:
        void init1();

        optional_http_response response_;
    };
}
