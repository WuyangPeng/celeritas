#include "buffer_guard.h"
#include "buffer_pool.h"

celeritas::buffer_guard::buffer_guard(buffer_pool_data buffer_data)
    : buffer_data_(std::move(buffer_data))
{
}

celeritas::buffer_guard::~buffer_guard()
{
    if (buffer_data_.data())
    {
        buffer_pool::release(std::move(buffer_data_));
    }
}

char* celeritas::buffer_guard::get()
{
    return buffer_data_.data().get();
}

size_t celeritas::buffer_guard::size() const
{
    return buffer_data_.size();
}
