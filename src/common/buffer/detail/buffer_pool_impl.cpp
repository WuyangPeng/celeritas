#include "buffer_pool_impl.h"
#include "common_buffer_internal_constant.h"

celeritas::buffer_pool_data celeritas::buffer_pool_impl::acquire(const size_t required_size)
{
    // 尝试从池中获取（加锁）
    if (auto buffer = try_acquire_from_pool(required_size);
        buffer)
    {
        // 由于使用std::optional，这里使用std::move避免复制
        return std::move(*buffer);
    }

    // 如果池中没有合适的，则创建新的
    return buffer_pool_data{ required_size };
}

void celeritas::buffer_pool_impl::release(buffer_pool_data buffer)
{
    std::lock_guard lock{ mutex_ };

    if (auto& bucket = pool_[buffer.size()];
        bucket.size() < max_idle_per_size)
    {
        bucket.emplace_front(std::move(buffer));
    }
}

void celeritas::buffer_pool_impl::reclaim(const duration_type idle_seconds)
{
    std::lock_guard lock{ mutex_ };

    const auto deadline = std::chrono::steady_clock::now() - idle_seconds;

    std::erase_if(pool_, [&](auto& entry) {
        return entry.second.last_take() < deadline;
    });
}

celeritas::buffer_pool_impl::buffer_pool_data_optional_type celeritas::buffer_pool_impl::try_acquire_from_pool(const size_t required_size)
{
    std::lock_guard lock{ mutex_ };

    if (const auto iter = pool_.lower_bound(required_size);
        iter != pool_.end())
    {
        auto& second = iter->second;
        auto buffer = second.extract();

        if (second.empty())
        {
            pool_.erase(iter);
        }
        return buffer;
    }

    return std::nullopt;
}