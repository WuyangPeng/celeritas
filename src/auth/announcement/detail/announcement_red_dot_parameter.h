#pragma once

#include "auth/core/detail/auth_parameter.h"
#include "message/basic/http_response.h"

namespace celeritas
{
    class announcement_red_dot_parameter final : public auth_parameter
    {
    public:
        using class_type = announcement_red_dot_parameter;
        using base_type = auth_parameter;

        explicit announcement_red_dot_parameter(const http_handle_parameter_shared_ptr& http_handle_parameter);

        [[nodiscard]] int64_t get_last_time() const;

        [[nodiscard]] bool is_failure() const override;

        [[nodiscard]] http_response get_response() const;

    private:
        void init1();

        optional_http_response response_;
        int64_t last_time_;
    };
}
