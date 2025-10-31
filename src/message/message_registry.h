#pragma once

#include "header.h"
#include "message_fwd.h"
#include "network/network_fwd.h"

#include <google/protobuf/message.h>
#include <shared_mutex>
#include <unordered_map>

namespace celeritas
{
    class message_registry : public std::enable_shared_from_this<message_registry>
    {
    public:
        using class_type = message_registry;
        using base_message_handler_shared_ptr = std::shared_ptr<base_message_handler>;
        using protobuf_message = google::protobuf::Message;
        using protobuf_message_shared_ptr = std::shared_ptr<google::protobuf::Message>;
        using message_registry_weak_ptr = std::weak_ptr<message_registry>;
        using session_shared_ptr = std::shared_ptr<session>;

        void registerHandler(const base_message_handler_shared_ptr& handler);

        [[nodiscard]] bool dispatch(const handle_parameter& handle_parameter, const protobuf_message& current_message);

    private:
        using registry_type = std::unordered_map<std::string, base_message_handler_shared_ptr>;
        using base_message_handler_optional_type = std::optional<base_message_handler_shared_ptr>;

        [[nodiscard]] base_message_handler_optional_type get_base_message_handler(const protobuf_message& current_message);

        registry_type registry_;
        std::shared_mutex mutex_;
    };
}