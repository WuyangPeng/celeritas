#include "http_request_session_write.h"

celeritas::http_request_session_write::http_request_session_write(socket_type& socket, std::string host, std::string path)
    : base_type{ socket, std::move(host) }, path_{ std::move(path) }
{
}

celeritas::http_request_session_write::bool_awaitable_type celeritas::http_request_session_write::do_one_write()
{
    co_return co_await do_one_write_request(path_);
}