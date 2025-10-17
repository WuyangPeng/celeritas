#include "message_header.h"
#include "detail/network_internal_fwd.h"

celeritas::message_header::message_header(const size_t header_size, const size_t body_size)
    : header_size{ boost::numeric_cast<uint16_t>(header_size) }, body_size{ boost::numeric_cast<uint32_t>(body_size) }
{
}

size_t celeritas::message_header::get_self_size()
{
    return sizeof(message_header);
}

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

void celeritas::message_header::host_to_network()
{
    header_type = htons(header_type);
    header_size = htons(header_size);
    body_size = htonl(body_size);
}