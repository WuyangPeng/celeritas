#include "buffer_pool_impl.h"
#include "common_internal_fwd.h"

celeritas::buffer_pool_data celeritas::buffer_pool_impl::acquire(size_t required_size)
{
    // 尝试从池中获取（加锁）
    if (auto buffer = try_acquire_from_pool(required_size);
        buffer.is_effective())
    {
        return buffer;
    }

    // 如果池中没有合适的，则创建新的
    return buffer_pool_data{ std::make_unique<char[]>(required_size), required_size };
}

void celeritas::buffer_pool_impl::release(buffer_pool_data buffer)
{
    std::lock_guard lock{ mutex_ };

    if (auto& entry = pool_[buffer.size()];
        entry.size() < max_idle_per_size)
    {
        entry.emplace_front(std::move(buffer));
    }
}

void celeritas::buffer_pool_impl::reclaim(const std::size_t idle_seconds)
{
    const auto deadline = std::chrono::steady_clock::now() - std::chrono::seconds(idle_seconds);

    std::lock_guard lock{ mutex_ };

    std::erase_if(pool_, [&](auto& entry) {
        return entry.second.last_take() < deadline;
    });
}

celeritas::buffer_pool_data celeritas::buffer_pool_impl::try_acquire_from_pool(const size_t required_size)
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

    return buffer_pool_data{};
}