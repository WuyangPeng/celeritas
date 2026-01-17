#include "session_base.h"
#include "common/buffer/buffer_pool.h"
#include "detail/write_protobuf_message.h"

celeritas::session_base::session_base(const server_network_type server_network_type, const int64_t session_id, session_callback session_callback)
    : server_network_type_{ server_network_type }, session_id_{ session_id }, session_callback_{ std::move(session_callback) }, instance_id_{}
{
}

void celeritas::session_base::write(const header& header, const protobuf_message_type& response)
{
    const write_protobuf_message write_protobuf_message{ shared_from_this(), header, response };

    if (auto buffer_guard = write_protobuf_message.get_buffer_guard();
        buffer_guard)
    {
        do_write(std::move(*buffer_guard));
    }
}

celeritas::session_base::void_awaitable_type celeritas::session_base::write_immediately(const std::string& response)
{
    const auto total_size = response.size();

    buffer_guard buffer_guard{ buffer_pool::acquire(total_size), total_size };

    buffer_guard.set(response);

    co_await do_write_immediately(std::move(buffer_guard));
}

int64_t celeritas::session_base::get_session_id() const noexcept
{
    return session_id_;
}

void celeritas::session_base::remove_session()
{
    session_callback_.remove_session(session_id_);
}

celeritas::server_network_type celeritas::session_base::get_server_network_type() const
{
    return server_network_type_;
}

void celeritas::session_base::set_instance_id(const std::string& instance_id)
{
    instance_id_ = instance_id;
}

std::string celeritas::session_base::get_instance_id() const
{
    return instance_id_;
}

celeritas::session_base::network_message_callback_weak_ptr celeritas::session_base::get_network_message_callback()
{
    return session_callback_.get_network_message_callback();
}

celeritas::session_callback celeritas::session_base::get_session_callback() const
{
    return session_callback_;
}