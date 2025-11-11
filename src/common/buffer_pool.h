#pragma once

#include "buffer_pool_data.h"
#include "common/detail/common_internal_fwd.h"

#include <chrono>

namespace celeritas
{
    class buffer_pool
    {
    public:
        using class_type = buffer_pool;
        using duration_type = std::chrono::milliseconds;

        // 获取一个缓冲区
        [[nodiscard]] static buffer_pool_data acquire(size_t required_size);

        // 归还一个缓冲区到池中
        static void release(buffer_pool_data buffer);

        static void reclaim(duration_type idle_seconds);

    private:
        [[nodiscard]] static buffer_pool_impl& get_buffer_pool();
    };
}
