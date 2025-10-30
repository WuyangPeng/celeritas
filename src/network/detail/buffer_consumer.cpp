#include "buffer_consumer.h"

celeritas::buffer_consumer::buffer_consumer(flat_buffer_type& buf) noexcept
    : buf_{ buf }
{
}

celeritas::buffer_consumer::~buffer_consumer() noexcept
{
    buf_.consume(buf_.size());
}