#include "message_header.h"

size_t celeritas::message_header::total_size() const
{
    return header_size + body_size;
}

bool celeritas::message_header::is_effective() const
{
    return header_size <= max_header_message_size && body_size <= max_message_size;
}

void celeritas::message_header::network_to_host()
{
    header_type = ntohs(header_type);
    header_size = ntohs(header_size);
    body_size = ntohl(body_size);
}