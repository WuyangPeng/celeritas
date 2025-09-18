#include "buffer_pool_data.h"

#include <utility>

celeritas::buffer_pool_data::buffer_pool_data(data_shared_ptr data, size_t size)
    : data_{ std::move(data) }, size_{ size }
{
}

celeritas::buffer_pool_data::data_shared_ptr& celeritas::buffer_pool_data::data()
{
    return data_;
}

size_t celeritas::buffer_pool_data::size() const
{
    return size_;
}
