#include "buffer_pool.h"

celeritas::buffer_pool_data celeritas::buffer_pool::acquire(size_t required_size)
{
    return get_buffer_pool().acquire(required_size);
}

void celeritas::buffer_pool::release(buffer_pool_data buffer)
{
    get_buffer_pool().release(std::move(buffer));
}

void celeritas::buffer_pool::reclaim(std::size_t idle_seconds)
{
    get_buffer_pool().reclaim(idle_seconds);
}

celeritas::buffer_pool_impl& celeritas::buffer_pool::get_buffer_pool()
{
    static buffer_pool_impl buffer_pool{};

    return buffer_pool;
}