#pragma once

#include "message/message_fwd.h"

#include <google/protobuf/message.h>
#include <shared_mutex>
#include <unordered_map>

namespace celeritas
{
    class protobuf_message_registry : public std::enable_shared_from_this<protobuf_message_registry>
    {
    public:
        using class_type = protobuf_message_registry;
        using protobuf_message = google::protobuf::Message;
        using protobuf_message_shared_ptr = std::shared_ptr<protobuf_message>;
        using base_message_handler_shared_ptr = std::shared_ptr<protobuf_base_message_handler>;
        using protobuf_handle_parameter_shared_ptr = std::shared_ptr<protobuf_handle_parameter>;

        explicit protobuf_message_registry(std::string_view server_type);

        void register_handler(const base_message_handler_shared_ptr& handler);

        [[nodiscard]] bool dispatch(const protobuf_handle_parameter_shared_ptr& handle_parameter, const protobuf_message& current_message);

    private:
        using registry_type = std::unordered_map<std::string, base_message_handler_shared_ptr>;
        using base_message_handler_optional_type = std::optional<base_message_handler_shared_ptr>;

        [[nodiscard]] base_message_handler_optional_type get_base_message_handler(const protobuf_message& current_message);

        registry_type registry_;
        std::shared_mutex mutex_;
        std::string_view server_type_;
    };
}