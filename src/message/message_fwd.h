#pragma once

namespace celeritas
{
    enum class game_error_type;

    class header;
    class celeritas_game_error;
    class http_response;
    class http_service_base;

    class protobuf_handle_parameter;
    class http_handle_parameter;

    class protobuf_base_message_handler;
    class http_base_message_handler;

    template <typename Message>
    class concrete_message_handler;

    class protobuf_message_registry;
    class http_message_registry;
}