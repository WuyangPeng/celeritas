#pragma once

#include "base_message_handler.h"

namespace celeritas
{
    template <typename Message>
    class concrete_message_handler : public base_message_handler
    {
    public:
        using class_type = concrete_message_handler;
        using base_type = base_message_handler;

        [[nodiscard]] std::string get_supported_type_name() const override;

        [[nodiscard]] bool handle(const handle_parameter& handle_parameter, const protobuf_message& current_message, const message_registry_weak_ptr& message_registry) override;

    protected:
        using message_type = Message;
        using message_registry_shared_ptr = std::shared_ptr<protobuf_message_registry>;
        using handler_function_type = bool (*)(const handle_parameter& handle_parameter, const message_type& current_message, const message_registry_shared_ptr& message_registry);

        [[nodiscard]] virtual bool handle_concrete(const handle_parameter& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry) = 0;

        void add_handler_function(int payload_case, handler_function_type handler_function);

        [[nodiscard]] handler_function_type get_handler_function(int payload_case) const;

        [[nodiscard]] bool handle_forward(const handle_parameter& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry);

        template <typename GetFunction>
        [[nodiscard]] static bool handle_dispatch(const handle_parameter& handle_parameter, const message_type& current_message, const message_registry_shared_ptr& message_registry, GetFunction get_function);

    private:
        using handler_container_type = std::map<int, handler_function_type>;

        handler_container_type handler_;
    };
}




