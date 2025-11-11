#pragma once

#include "header.h"
#include "message_fwd.h"
#include "network/network_fwd.h"

#include <shared_mutex>
#include <unordered_map>

namespace celeritas
{
    class http_message_registry : public std::enable_shared_from_this<http_message_registry>
    {
    public:
        using class_type = http_message_registry;
        using http_base_message_handler_shared_ptr = std::shared_ptr<http_base_message_handler>;
        using http_message_registry_weak_ptr = std::weak_ptr<http_message_registry>;
        using session_shared_ptr = std::shared_ptr<session_base>;

        void registerHandler(const http_base_message_handler_shared_ptr& handler);

        [[nodiscard]] bool dispatch(const http_handle_parameter& handle_parameter);

    private:
        using registry_type = std::unordered_map<std::string, http_base_message_handler_shared_ptr>;
        using http_base_message_handler_optional_type = std::optional<http_base_message_handler_shared_ptr>;

        [[nodiscard]] http_base_message_handler_optional_type get_http_base_message_handler(const http_handle_parameter& handle_parameter);

        registry_type registry_;
        std::shared_mutex mutex_;
    };
}