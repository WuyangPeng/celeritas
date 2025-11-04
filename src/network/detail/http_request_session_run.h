#pragma once

#include "http_session_run.h"

namespace celeritas
{
    class http_request_session_run : public http_session_run
    {
    public:
        using class_type = http_request_session_run;
        using base_type = http_session_run;

        http_request_session_run(socket_type& socket, int64_t session_id, session_callback session_callback);

        [[nodiscard]] void_awaitable_type handle_one_message() override;
    };
}