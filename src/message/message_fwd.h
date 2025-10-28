#pragma once

namespace celeritas
{
    template <typename Message>
    class concrete_message_handler;

    class base_message_handler;
    class header;
    class handle_parameter;
    class message_registry;
    class request_message_handler;
    class response_message_handler;
    class service_request_message_handler;
    class http_handle_parameter;
    class http_base_message_handler;
    class http_message_registry;
    class health_check_http_message_handler;
}