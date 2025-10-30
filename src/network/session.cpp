#include "message_header.h"
#include "session.h"
#include "common/buffer_pool.h"
#include "detail/write_protobuf_message.h"

celeritas::session::session(const int64_t session_id, session_callback session_callback)
    : session_id_{ session_id }, session_callback_{ std::move(session_callback) }
{
}

void celeritas::session::write(const header& header, const protobuf_message_type& response)
{
    const write_protobuf_message write_protobuf_message{ shared_from_this(), header, response };

    if (auto buffer_guard = write_protobuf_message.get_buffer_guard();
        buffer_guard.is_effective())
    {
        do_write(std::move(buffer_guard));
    }
}

void celeritas::session::write(const std::string& response)
{
    const auto total_size = response.size();

    buffer_guard buffer_guard{ buffer_pool::acquire(total_size), total_size };

    buffer_guard.set(response);

    do_write(std::move(buffer_guard));
}

int64_t celeritas::session::get_session_id() const noexcept
{
    return session_id_;
}

void celeritas::session::remove_session()
{
    session_callback_.remove_session(session_id_);
}

celeritas::session::network_message_callback_weak_ptr celeritas::session::get_network_message_callback()
{
    return session_callback_.get_network_message_callback();
}

celeritas::session_callback celeritas::session::get_session_callback() const
{
    return session_callback_;
}