#pragma once

#include "handle_parameter.h"
#include "protobuf_message_registry.h"
#include "network/message_header.h"

#include <google/protobuf/message.h>
#include <string>

namespace celeritas
{
    class base_message_handler
    {
    public:
        using class_type = base_message_handler;
        using protobuf_message = google::protobuf::Message;
        using protobuf_message_shared_ptr = std::shared_ptr<protobuf_message>;
        using message_registry_weak_ptr = std::weak_ptr<protobuf_message_registry>;
        using session_shared_ptr = std::shared_ptr<session_base>;

        base_message_handler() noexcept = default;

        virtual ~base_message_handler() noexcept = default;

        base_message_handler(const base_message_handler& rhs) = default;

        base_message_handler& operator=(const base_message_handler& rhs) = default;

        base_message_handler(base_message_handler&& rhs) noexcept = default;

        base_message_handler& operator=(base_message_handler&& rhs) noexcept = default;

        [[nodiscard]] virtual std::string get_supported_type_name() const = 0;

        [[nodiscard]] virtual bool handle(const handle_parameter& handle_parameter, const protobuf_message& current_message, const message_registry_weak_ptr& message_registry) = 0;
    };
}
