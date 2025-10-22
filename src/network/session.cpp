#include "message_header.h"
#include "session.h"
#include "common/buffer_pool.h"
#include "common/logger.h"

celeritas::session::session(const int64_t session_id, session_callback session_callback)
    : session_id_{ session_id }, session_callback_{ std::move(session_callback) }
{
}

void celeritas::session::write(const header& header, const google::protobuf::Message& response)
{
    const auto header_request = header.get_message();

    message_header message_header{ header_request->ByteSizeLong(), response.ByteSizeLong() };

    const auto header_size = message_header::get_self_size();

    const auto total_size = message_header.get_total_size() + header_size;
    buffer_guard buffer_guard{ buffer_pool::acquire(total_size) };
    buffer_guard.set_effective_size(total_size);

    message_header.host_to_network();

    std::memcpy(buffer_guard.get(), &message_header, header_size);

    if (!header_request->SerializeToArray(buffer_guard.get(header_size), boost::numeric_cast<int>(header_request->ByteSizeLong())))
    {
        LOG_CHANNEL(network_channel, error) << "序列化失败！";
        return;
    }

    if (const auto offset = header_size + header_request->ByteSizeLong();
        !response.SerializeToArray(buffer_guard.get(offset), boost::numeric_cast<int>(response.ByteSizeLong())))
    {
        LOG_CHANNEL(network_channel, error) << "序列化失败！";
        return;
    }

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