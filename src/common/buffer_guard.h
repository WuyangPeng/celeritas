#pragma once

#include "buffer_pool_data.h"

namespace celeritas
{
    class buffer_guard
    {
    public:
        using class_type = buffer_guard;

        buffer_guard() noexcept = default;
        explicit buffer_guard(buffer_pool_data buffer_data);

        buffer_guard(const buffer_guard& rhs) = delete;
        buffer_guard& operator=(const buffer_guard& rhs) = delete;
        buffer_guard(buffer_guard&& rhs) noexcept;
        buffer_guard& operator=(buffer_guard&& rhs) noexcept;

        ~buffer_guard() noexcept;

        [[nodiscard]] char* get();
        [[nodiscard]] size_t size() const;
        [[nodiscard]] size_t get_effective_size() const;
        void set_effective_size(size_t size);

    private:
        buffer_pool_data buffer_data_;
        size_t effective_size_;
    };
}