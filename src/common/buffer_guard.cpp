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
    auto result{ std::move(rhs) };
    std::swap(buffer_data_, result.buffer_data_);

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

void celeritas::buffer_guard::swap(buffer_guard& rhs)
{
    std::swap(buffer_data_, rhs.buffer_data_);
}
