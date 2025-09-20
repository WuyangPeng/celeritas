#include "message_header.h"
#include "detail/network_internal_fwd.h"

size_t celeritas::message_header::get_total_size() const
{
    return header_size + body_size;
}

bool celeritas::message_header::is_effective() const
{
    return header_size <= max_header_message_size && body_size <= max_message_size;
}

uint16_t celeritas::message_header::get_header_type() const
{
    return header_type;
}

uint16_t celeritas::message_header::get_header_size() const
{
    return header_size;
}

uint32_t celeritas::message_header::get_body_size() const
{
    return body_size;
}

void celeritas::message_header::network_to_host()
{
    header_type = ntohs(header_type);
    header_size = ntohs(header_size);
    body_size = ntohl(body_size);
}