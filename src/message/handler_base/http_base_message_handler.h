#pragma once

#include "message/message_fwd.h"

#include <memory>
#include <string>

namespace celeritas
{
    class http_base_message_handler
    {
    public:
        using class_type = http_base_message_handler;
        using http_message_registry_weak_ptr = std::weak_ptr<http_message_registry>;
        using http_handle_parameter_shared_ptr = std::shared_ptr<http_handle_parameter>;

        http_base_message_handler() noexcept = default;

        virtual ~http_base_message_handler() noexcept = default;

        http_base_message_handler(const http_base_message_handler& rhs) = default;

        http_base_message_handler& operator=(const http_base_message_handler& rhs) = default;

        http_base_message_handler(http_base_message_handler&& rhs) noexcept = default;

        http_base_message_handler& operator=(http_base_message_handler&& rhs) noexcept = default;

        [[nodiscard]] virtual std::string get_supported_type_name() const = 0;

        [[nodiscard]] virtual bool handle(const http_handle_parameter_shared_ptr& handle_parameter, const http_message_registry_weak_ptr& message_registry) = 0;
    };
}
