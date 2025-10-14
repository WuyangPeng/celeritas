#pragma once

#include "base_message_handler.h"
#include "header.h"

#include <google/protobuf/message.h>
#include <shared_mutex>
#include <unordered_map>

namespace celeritas
{
    class message_registry
    {
    public:
        using class_type = message_registry;
        using base_message_handler_shared_ptr = std::shared_ptr<base_message_handler>;
        using protobuf_message_shared_ptr = std::shared_ptr<google::protobuf::Message>;

        void registerHandler(const base_message_handler_shared_ptr& handler);

        [[nodiscard]] bool dispatch(const header& header, const protobuf_message_shared_ptr& message);

    private:
        using registry_type = std::unordered_map<std::string, base_message_handler_shared_ptr>;

        registry_type registry_;
        std::shared_mutex mutex_;
    };
}