#include "buffer_pool_impl.h"

celeritas::buffer_pool_data celeritas::buffer_pool_impl::acquire(size_t required_size)
{
    std::lock_guard lock{ mutex_ };

    if (const auto iter = pool_.lower_bound(required_size);
        iter != pool_.end())
    {
        // 找到最匹配的缓冲区，并从列表中取出
        auto& second = iter->second;

        auto buffer = std::move(second.front());
        second.pop_front();

        if (second.empty())
        {
            pool_.erase(iter);
        }
        return buffer;
    }

    // 如果池中没有合适的，则创建新的
    return buffer_pool_data{ std::make_unique<char[]>(required_size), required_size };
}

void celeritas::buffer_pool_impl::release(buffer_pool_data buffer)
{
    std::lock_guard lock{ mutex_ };
    pool_[buffer.size()].emplace_front(std::move(buffer));
}