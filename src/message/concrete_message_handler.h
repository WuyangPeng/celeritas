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

        [[nodiscard]] bool handle(const header& header, const protobuf_message_shared_ptr& message) override;

    protected:
        using message_shared_ptr = std::shared_ptr<Message>;

        virtual bool handle_concrete(const header& header, const message_shared_ptr& message) = 0;
    };
}




