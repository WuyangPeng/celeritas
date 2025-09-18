#include "buffer_pool_bucket.h"

size_t celeritas::buffer_pool_bucket::size() const noexcept
{
    return data_.size();
}

bool celeritas::buffer_pool_bucket::empty() const noexcept
{
    return data_.empty();
}

void celeritas::buffer_pool_bucket::emplace_front(buffer_pool_data buffer)
{
    data_.emplace_front(std::move(buffer));
}

celeritas::buffer_pool_data celeritas::buffer_pool_bucket::extract()
{
    auto buffer = std::move(data_.front());
    data_.pop_front();
    last_take_ = std::chrono::steady_clock::now();

    return buffer;
}

std::chrono::steady_clock::time_point celeritas::buffer_pool_bucket::last_take() const
{
    return last_take_;
}