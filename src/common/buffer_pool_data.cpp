#include "buffer_pool_data.h"

#include <utility>

celeritas::buffer_pool_data::buffer_pool_data(data_unique_ptr data, size_t size)
    : data_{ std::move(data) }, size_{ size }
{
}

celeritas::buffer_pool_data::buffer_pool_data(buffer_pool_data&& rhs) noexcept
    : data_{ std::move(rhs.data_) }, size_{ rhs.size_ }
{
}

celeritas::buffer_pool_data& celeritas::buffer_pool_data::operator=(buffer_pool_data&& rhs) noexcept
{
    data_ = std::move(rhs.data_);
    size_ = rhs.size_;

    return *this;
}

char* celeritas::buffer_pool_data::data() noexcept
{
    if (is_effective())
    {
        return data_.get();
    }

    return nullptr;
}

size_t celeritas::buffer_pool_data::size() const noexcept
{
    return size_;
}

bool celeritas::buffer_pool_data::is_effective() const noexcept
{
    return data_ != nullptr;
}
