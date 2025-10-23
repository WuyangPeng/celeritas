#include "header.h"

celeritas::header::header(const proto::common::empty_message_header& empty_message_header)
    : rpc{}, user_id{}, code{}
{
}

celeritas::header::header(const proto::common::server_message_header& server_message_header)
    : rpc{}, user_id{ server_message_header.user_id() }, code{}
{
}

celeritas::header::header(const proto::common::client_message_header& client_message_header)
    : rpc{ client_message_header.rpc() }, user_id{}, code{}
{
}

celeritas::header::header(const proto::common::gateway_message_header& gateway_message_header)
    : rpc{ gateway_message_header.rpc() }, user_id{ gateway_message_header.user_id() }, code{}
{
}

celeritas::header::header(const proto::common::to_gateway_message_header& to_gateway_message_header)
    : rpc{ to_gateway_message_header.rpc() }, user_id{ to_gateway_message_header.user_id() }, code{ to_gateway_message_header.code() }
{
}

celeritas::header::message_shared_ptr celeritas::header::get_message() const
{
    if (code > 0)
    {
        auto header = std::make_shared<proto::common::header>();
        auto to_gateway_message_header = header->mutable_to_gateway();

        to_gateway_message_header->set_user_id(user_id);
        to_gateway_message_header->set_rpc(rpc);
        to_gateway_message_header->set_code(code);

        return header;
    }

    if (user_id > 0 && rpc > 0)
    {
        auto header = std::make_shared<proto::common::header>();
        auto gateway_message_header = header->mutable_gateway();

        gateway_message_header->set_user_id(user_id);
        gateway_message_header->set_rpc(rpc);

        return header;
    }

    if (user_id > 0)
    {
        auto header = std::make_shared<proto::common::header>();
        auto server_message_header = header->mutable_server();

        server_message_header->set_user_id(user_id);

        return header;
    }

    if (rpc > 0)
    {
        auto header = std::make_shared<proto::common::header>();
        auto client_message_header = header->mutable_client();

        client_message_header->set_rpc(rpc);

        return header;
    }

    auto header = std::make_shared<proto::common::header>();

    header->mutable_empty();

    return header;
}