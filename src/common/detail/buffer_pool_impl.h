#pragma once

#include "buffer_pool_bucket.h"
#include "common/buffer_pool_data.h"

#include <map>
#include <memory>

namespace celeritas
{
    class buffer_pool_impl
    {
    public:
        using class_type = buffer_pool_impl;

        // 获取一个缓冲区
        [[nodiscard]] buffer_pool_data acquire(size_t required_size);

        // 归还一个缓冲区到池中
        void release(buffer_pool_data buffer);

        void reclaim(std::size_t idle_seconds);

    private:
        using pool_type = std::map<size_t, buffer_pool_bucket>;

        [[nodiscard]] buffer_pool_data try_acquire_from_pool(size_t required_size);

        pool_type pool_;
        std::mutex mutex_;
    };
}
