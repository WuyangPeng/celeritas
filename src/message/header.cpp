#include "header.h"

celeritas::header::header(const empty_message_header_type& empty_message_header)
    : rpc{}, user_id{}, code{}
{
}

celeritas::header::header(const server_message_header_type& server_message_header)
    : rpc{}, user_id{ server_message_header.user_id() }, code{}
{
}

celeritas::header::header(const client_message_header_type& client_message_header)
    : rpc{ client_message_header.rpc() }, user_id{}, code{}
{
}

celeritas::header::header(const gateway_message_header_type& gateway_message_header)
    : rpc{ gateway_message_header.rpc() }, user_id{ gateway_message_header.user_id() }, code{}
{
}

celeritas::header::header(const to_gateway_message_header_type& to_gateway_message_header)
    : rpc{ to_gateway_message_header.rpc() }, user_id{ to_gateway_message_header.user_id() }, code{ to_gateway_message_header.code() }
{
}

celeritas::header::message_shared_ptr celeritas::header::get_message() const
{
    if (code > 0)
    {
        return get_to_gateway_message();
    }

    if (user_id > 0 && rpc > 0)
    {
        return get_gateway_message();
    }

    if (user_id > 0)
    {
        return get_server_message();
    }

    if (rpc > 0)
    {
        return get_client_message();
    }

    return get_empty_message();
}

celeritas::header::message_shared_ptr celeritas::header::get_to_gateway_message() const
{
    auto header = std::make_shared<proto::common::header>();
    auto* to_gateway_message_header = header->mutable_to_gateway();

    to_gateway_message_header->set_user_id(user_id);
    to_gateway_message_header->set_rpc(rpc);
    to_gateway_message_header->set_code(code);

    return header;
}

celeritas::header::message_shared_ptr celeritas::header::get_gateway_message() const
{
    auto header = std::make_shared<proto::common::header>();
    auto* gateway_message_header = header->mutable_gateway();

    gateway_message_header->set_user_id(user_id);
    gateway_message_header->set_rpc(rpc);

    return header;
}

celeritas::header::message_shared_ptr celeritas::header::get_server_message() const
{
    auto header = std::make_shared<proto::common::header>();
    auto* server_message_header = header->mutable_server();

    server_message_header->set_user_id(user_id);

    return header;
}

celeritas::header::message_shared_ptr celeritas::header::get_client_message() const
{
    auto header = std::make_shared<proto::common::header>();
    auto* client_message_header = header->mutable_client();

    client_message_header->set_rpc(rpc);

    return header;
}

celeritas::header::message_shared_ptr celeritas::header::get_empty_message() const
{
    auto header = std::make_shared<proto::common::header>();
    header->mutable_empty();

    return header;
}