#include "header.h"

celeritas::header::header(const proto::common::empty_message_header& empty_message_header)
    : rpc{}, user_id{}, code{}
{
}

celeritas::header::header(const proto::common::server_message_header& server_message_header)
    : rpc{}, user_id{ server_message_header.user_id() }, code{ server_message_header.code() }
{
}

celeritas::header::header(const proto::common::client_message_header& client_message_header)
    : rpc{ client_message_header.rpc() }, user_id{}, code{}
{
}

celeritas::header::message_shared_ptr celeritas::header::get_message() const
{
    if (user_id > 0)
    {
        auto server_message_header = std::make_shared<proto::common::server_message_header>();

        server_message_header->set_user_id(user_id);
        server_message_header->set_code(code);

        return server_message_header;
    }

    if (rpc > 0)
    {
        auto client_message_header = std::make_shared<proto::common::client_message_header>();

        client_message_header->set_rpc(rpc);

        return client_message_header;
    }

    return std::make_shared<proto::common::empty_message_header>();
}