#pragma once

#include "protobuf_handle_parameter.h"
#include "protobuf_message_registry.h"

#include <google/protobuf/message.h>
#include <string>

namespace celeritas
{
    class protobuf_base_message_handler
    {
    public:
        using class_type = protobuf_base_message_handler;
        using protobuf_message = google::protobuf::Message;
        using protobuf_message_shared_ptr = std::shared_ptr<protobuf_message>;
        using message_registry_weak_ptr = std::weak_ptr<protobuf_message_registry>;
        using session_shared_ptr = std::shared_ptr<session_base>;

        protobuf_base_message_handler() noexcept = default;

        virtual ~protobuf_base_message_handler() noexcept = default;

        protobuf_base_message_handler(const protobuf_base_message_handler& rhs) = default;

        protobuf_base_message_handler& operator=(const protobuf_base_message_handler& rhs) = default;

        protobuf_base_message_handler(protobuf_base_message_handler&& rhs) noexcept = default;

        protobuf_base_message_handler& operator=(protobuf_base_message_handler&& rhs) noexcept = default;

        [[nodiscard]] virtual std::string get_supported_type_name() const = 0;

        [[nodiscard]] virtual bool handle(const protobuf_handle_parameter& handle_parameter, const protobuf_message& current_message, const message_registry_weak_ptr& message_registry) = 0;
    };
}
