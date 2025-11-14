#pragma once

#include "message/message_fwd.h"

#include <memory>

namespace celeritas
{
    class register_handler_helper
    {
    public:
        using class_type = register_handler_helper;
        using message_registry_shared_ptr = std::shared_ptr<protobuf_message_registry>;
        using http_message_registry_shared_ptr = std::shared_ptr<http_message_registry>;

        explicit register_handler_helper(message_registry_shared_ptr message_registry, http_message_registry_shared_ptr http_message_registry);

        void register_handler() const;

    private:
        message_registry_shared_ptr message_registry_;
        http_message_registry_shared_ptr http_message_registry_;
    };
}