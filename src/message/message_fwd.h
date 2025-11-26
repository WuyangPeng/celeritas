#pragma once

namespace celeritas
{
    class header;
    class protobuf_handle_parameter;
    class http_handle_parameter;

    class protobuf_base_message_handler;
    class http_base_message_handler;

    template <typename Message>
    class concrete_message_handler;

    class protobuf_message_registry;
    class http_message_registry;
}