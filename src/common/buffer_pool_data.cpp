#include "buffer_pool_data.h"
#include "celeritas_error.h"

celeritas::buffer_pool_data::buffer_pool_data(size_t size)
    : data_(size)
{
}

char* celeritas::buffer_pool_data::data()
{
    if (is_effective())
    {
        return data_.data();
    }

    throw celeritas_error("unsupported buffer pool data size");
}

const char* celeritas::buffer_pool_data::data() const
{
    if (is_effective())
    {
        return data_.data();
    }

    return nullptr;
}

size_t celeritas::buffer_pool_data::size() const noexcept
{
    return data_.size();
}

bool celeritas::buffer_pool_data::is_effective() const noexcept
{
    return !data_.empty();
}

char* celeritas::buffer_pool_data::get(const size_t offset)
{
    if (is_effective() && offset < data_.size())
    {
        return &data_.at(offset);
    }

    throw celeritas_error("unsupported buffer pool data offset");
}
