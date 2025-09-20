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
        using time_point_type = std::chrono::steady_clock::time_point;

        buffer_pool_bucket() noexcept = default;

        ~buffer_pool_bucket() noexcept = default;

        buffer_pool_bucket(const buffer_pool_bucket& rhs) = delete;

        buffer_pool_bucket& operator=(const buffer_pool_bucket& rhs) = delete;

        buffer_pool_bucket(buffer_pool_bucket&& rhs) noexcept = delete;

        buffer_pool_bucket& operator=(buffer_pool_bucket&& rhs) noexcept = delete;

        [[nodiscard]] size_t size() const noexcept;

        [[nodiscard]] bool empty() const noexcept;

        void emplace_front(buffer_pool_data buffer);

        [[nodiscard]] buffer_pool_data extract();

        [[nodiscard]] time_point_type last_take() const;

    private:
        std::list<buffer_pool_data> data_;
        time_point_type last_take_ = std::chrono::steady_clock::now();
    };
}
