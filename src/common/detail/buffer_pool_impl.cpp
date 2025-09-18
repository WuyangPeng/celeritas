#include "buffer_pool_impl.h"

celeritas::buffer_pool_data celeritas::buffer_pool_impl::acquire(size_t required_size)
{
    std::lock_guard lock{ mutex_ };

    if (!pool_.empty())
    {
        // 优先复用大小匹配的缓冲区
        for (auto it = pool_.begin(); it != pool_.end(); ++it)
        {
            if (it->size() >= required_size)
            {
                auto buffer = std::move(*it);
                pool_.erase(it);
                return buffer;
            }
        }
    }

    // 如果池中没有合适的，则创建新的
    return buffer_pool_data{ std::make_shared<char[]>(required_size), required_size };
}

void celeritas::buffer_pool_impl::release(buffer_pool_data buffer)
{
    std::lock_guard lock{ mutex_ };
    pool_.emplace_back(std::move(buffer));
}