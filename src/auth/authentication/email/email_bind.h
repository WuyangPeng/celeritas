#pragma once

#include "auth/core/auth_bind.h"
#include "message/http_handle_parameter.h"

#include <boost/asio/awaitable.hpp>

namespace celeritas {
    class email_bind final : public auth_bind {
    public:
        using class_type = email_bind;
        using base_type = auth_bind;

        explicit email_bind(http_handle_parameter handle_parameter);

        [[nodiscard]] void_awaitable_type response() override;
    };
}