#pragma once

#include "buffer_pool_data.h"

namespace celeritas
{
    class buffer_guard
    {
    public:
        using class_type = buffer_guard;

        explicit buffer_guard(buffer_pool_data buffer_data);

        buffer_guard(const buffer_guard&) = delete;
        buffer_guard& operator=(const buffer_guard&) = delete;
        buffer_guard(buffer_guard&& rhs) noexcept;
        buffer_guard& operator=(buffer_guard&& rhs) noexcept;

        ~buffer_guard() noexcept;

        [[nodiscard]] char* get();
        [[nodiscard]] size_t size() const;

    private:
        buffer_pool_data buffer_data_;
    };
}