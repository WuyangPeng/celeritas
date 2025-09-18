#include "buffer_guard.h"
#include "buffer_pool.h"

celeritas::buffer_guard::buffer_guard(buffer_pool_data buffer_data)
    : buffer_data_(std::move(buffer_data))
{
}

celeritas::buffer_guard::buffer_guard(buffer_guard&& rhs) noexcept
    : buffer_data_(std::move(rhs.buffer_data_))
{
}

celeritas::buffer_guard& celeritas::buffer_guard::operator=(buffer_guard&& rhs) noexcept
{
    if (this != &rhs)
    {
        buffer_pool_data old_data = std::move(buffer_data_);
        buffer_data_ = std::move(rhs.buffer_data_);

        if (old_data.data())
        {
            buffer_pool::release(std::move(old_data));
        }
    }

    return *this;
}

celeritas::buffer_guard::~buffer_guard() noexcept
{
    if (buffer_data_.data())
    {
        buffer_pool::release(std::move(buffer_data_));
    }
}

char* celeritas::buffer_guard::get()
{
    return buffer_data_.data();
}

size_t celeritas::buffer_guard::size() const
{
    return buffer_data_.size();
}
