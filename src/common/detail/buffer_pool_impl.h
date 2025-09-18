#pragma once

#include "common/buffer_pool_data.h"

#include <list>
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

    private:
        // 使用 std::list 以便快速删除
        std::list<buffer_pool_data> pool_;
        std::mutex mutex_;
    };
}
