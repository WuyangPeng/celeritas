#include "write_protobuf_message.h"
#include "common/buffer/buffer_pool.h"
#include "common/logging/logger.h"
#include "network/core/message_header.h"

#include  <boost/numeric/conversion/cast.hpp>

celeritas::write_protobuf_message::write_protobuf_message(const session_shared_ptr& session, const header& header, const protobuf_message_type& response)
    : session_{ session }, header_{ header }, response_{ response }
{
}

celeritas::write_protobuf_message::buffer_guard_optional_type celeritas::write_protobuf_message::get_buffer_guard() const
{
    const auto header_request = header_.get_message();

    message_header message_header{ header_request->ByteSizeLong(), response_.ByteSizeLong() };

    const auto header_size = message_header::get_self_size();
    const auto total_size = message_header.get_total_size() + header_size;

    buffer_guard buffer_guard{ buffer_pool::acquire(total_size), total_size };
    message_header.host_to_network();
    buffer_guard.set(message_header.get_span());

    if (!write(header_request, header_size, buffer_guard))
    {
        return std::nullopt;
    }

    return buffer_guard;
}

bool celeritas::write_protobuf_message::write(const message_shared_ptr& header_request, size_t header_size, buffer_guard& buffer_guard) const
{
    if (!header_request->SerializeToArray(buffer_guard.get(header_size), boost::numeric_cast<int>(header_request->ByteSizeLong())))
    {
        LOG_CHANNEL(network_channel, error) << "序列化失败！";
        return false;
    }

    if (const auto offset = header_size + header_request->ByteSizeLong();
        !response_.SerializeToArray(buffer_guard.get(offset), boost::numeric_cast<int>(response_.ByteSizeLong())))
    {
        LOG_CHANNEL(network_channel, error) << "序列化失败！";
        return false;
    }

    return true;
}