#include "game_error_type.h"
#include "header.h"

celeritas::header::header(const int32_t rpc, const int32_t code)
    : rpc_{ rpc }, user_id_{}, code_{ code }
{
}

celeritas::header::header(const int32_t rpc, const int64_t user_id, const int32_t code)
    : rpc_{ rpc }, user_id_{ user_id }, code_{ code }
{
}

celeritas::header::header(const int32_t rpc, const int64_t user_id)
    : rpc_{ rpc }, user_id_{ user_id }, code_{ static_cast<int>(game_error_type::success) }
{
}

celeritas::header::header(const int64_t user_id)
    : rpc_{ 0 }, user_id_{ user_id }, code_{ static_cast<int>(game_error_type::success) }
{
}

celeritas::header::header(const empty_message_header_type& empty_message_header)
    : rpc_{}, user_id_{}, code_{}
{
}

celeritas::header::header(const server_message_header_type& server_message_header)
    : rpc_{}, user_id_{ server_message_header.user_id() }, code_{}
{
}

celeritas::header::header(const client_message_header_type& client_message_header)
    : rpc_{ client_message_header.rpc() }, user_id_{}, code_{}
{
}

celeritas::header::header(const gateway_message_header_type& gateway_message_header)
    : rpc_{ gateway_message_header.rpc() }, user_id_{ gateway_message_header.user_id() }, code_{}
{
}

celeritas::header::header(const to_gateway_message_header_type& to_gateway_message_header)
    : rpc_{ to_gateway_message_header.rpc() }, user_id_{ to_gateway_message_header.user_id() }, code_{ to_gateway_message_header.code() }
{
}

celeritas::header::message_shared_ptr celeritas::header::get_message() const
{
    if (code_ > 0)
    {
        return get_to_gateway_message();
    }

    if (user_id_ > 0 && rpc_ > 0)
    {
        return get_gateway_message();
    }

    if (user_id_ > 0)
    {
        return get_server_message();
    }

    if (rpc_ > 0)
    {
        return get_client_message();
    }

    return get_empty_message();
}

int32_t celeritas::header::get_rpc() const
{
    return rpc_;
}

int64_t celeritas::header::get_user_id() const
{
    return user_id_;
}

int32_t celeritas::header::get_code() const
{
    return code_;
}

celeritas::header::message_shared_ptr celeritas::header::get_to_gateway_message() const
{
    auto header = std::make_shared<proto::common::header>();
    auto* to_gateway_message_header = header->mutable_to_gateway();

    to_gateway_message_header->set_user_id(user_id_);
    to_gateway_message_header->set_rpc(rpc_);
    to_gateway_message_header->set_code(code_);

    return header;
}

celeritas::header::message_shared_ptr celeritas::header::get_gateway_message() const
{
    auto header = std::make_shared<proto::common::header>();
    auto* gateway_message_header = header->mutable_gateway();

    gateway_message_header->set_user_id(user_id_);
    gateway_message_header->set_rpc(rpc_);

    return header;
}

celeritas::header::message_shared_ptr celeritas::header::get_server_message() const
{
    auto header = std::make_shared<proto::common::header>();
    auto* server_message_header = header->mutable_server();

    server_message_header->set_user_id(user_id_);

    return header;
}

celeritas::header::message_shared_ptr celeritas::header::get_client_message() const
{
    auto header = std::make_shared<proto::common::header>();
    auto* client_message_header = header->mutable_client();

    client_message_header->set_rpc(rpc_);

    return header;
}

celeritas::header::message_shared_ptr celeritas::header::get_empty_message() const
{
    auto header = std::make_shared<proto::common::header>();
    header->mutable_empty();

    return header;
}