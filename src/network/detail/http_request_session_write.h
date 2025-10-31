#pragma once

#include "http_session_write.h"

namespace celeritas
{
    class http_request_session_write final : public http_session_write
    {
    public:
        using class_type = http_request_session_write;
        using base_type = http_session_write;

        explicit http_request_session_write(socket_type& socket, std::string host, std::string path);

    private:
        [[nodiscard]] bool_awaitable_type do_one_write() override;

        std::string path_;
    };
}