#include "read_buffer_guard.h"
#include "common/buffer/buffer_pool.h"
#include "common/logger/logger.h"

celeritas::read_buffer_guard::read_buffer_guard(const payload_data_type& payload_data)
    : payload_data_{ payload_data }, message_header_{}, is_successful_{ true }
{
    set_message_header();
}

celeritas::read_buffer_guard::buffer_guard_optional_type celeritas::read_buffer_guard::get_buffer_guard()
{
    if (is_successful_)
    {
        return std::nullopt;
    }

    const auto total_size = message_header_.get_total_size();
    const auto self_size = message_header::get_self_size();

    if (payload_data_.size() < total_size + self_size)
    {
        LOG_CHANNEL(network_channel, error) << "websocket frame incomplete";
        is_successful_ = false;
        return std::nullopt;
    }

    buffer_guard buffer_guard{ buffer_pool::acquire(total_size), total_size };

    const auto body_view = payload_data_.subspan(self_size, total_size);
    buffer_guard.set(body_view);

    return buffer_guard;
}

celeritas::message_header celeritas::read_buffer_guard::get_message_header() const
{
    return message_header_;
}

void celeritas::read_buffer_guard::set_message_header()
{
    const auto self_size = message_header::get_self_size();
    if (payload_data_.size() < self_size)
    {
        LOG_CHANNEL(network_channel, error) << "websocket frame too small for header";
        is_successful_ = false;
        return;
    }

    const auto header_view = payload_data_.subspan(0, self_size);

    message_header_.set_span(header_view);
    message_header_.network_to_host();
}