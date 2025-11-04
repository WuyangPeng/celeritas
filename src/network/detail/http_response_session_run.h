#pragma once

#include "http_session_run.h"

namespace celeritas
{
    class http_response_session_run : public http_session_run
    {
    public:
        using class_type = http_response_session_run;
        using base_type = http_session_run;

        http_response_session_run(socket_type& socket, int64_t session_id, session_callback session_callback, std::string path);

        [[nodiscard]] void_awaitable_type handle_one_message() override;

    private:
        std::string path_;
    };
}