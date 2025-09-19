#pragma once

#include "common/buffer_pool_data.h"

#include <chrono>
#include <list>

namespace celeritas
{
    class buffer_pool_bucket
    {
    public:
        using class_type = buffer_pool_bucket;

        [[nodiscard]] size_t size() const noexcept;
        [[nodiscard]] bool empty() const noexcept;
        void emplace_front(buffer_pool_data buffer);
        [[nodiscard]] buffer_pool_data extract();
        [[nodiscard]] std::chrono::steady_clock::time_point last_take() const;

    private:
        std::list<buffer_pool_data> data_;
        std::chrono::steady_clock::time_point last_take_ = std::chrono::steady_clock::now();
    };
}
