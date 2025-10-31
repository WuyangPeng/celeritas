#include "buffer_consumer.h"

celeritas::buffer_consumer::buffer_consumer(flat_buffer_type& buffer) noexcept
    : buffer_{ buffer }
{
}

celeritas::buffer_consumer::~buffer_consumer() noexcept
{
    buffer_.consume(buffer_.size());
}