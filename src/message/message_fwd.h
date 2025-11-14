#pragma once

namespace celeritas
{
    template <typename Message>
    class concrete_message_handler;

    class header;
    class protobuf_base_message_handler;
    class protobuf_handle_parameter;
    class protobuf_message_registry;
    class http_handle_parameter;
    class http_base_message_handler;
    class http_message_registry;
}