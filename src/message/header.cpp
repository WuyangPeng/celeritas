#include "header.h"

celeritas::header::header(const proto::server_message_header& server_message_header)
    : rpc{}, user_id{ server_message_header.user_id() }, code{ server_message_header.code() }
{
}

celeritas::header::header(const proto::client_message_header& client_message_header)
    : rpc{ client_message_header.rpc() }, user_id{}, code{}
{
}