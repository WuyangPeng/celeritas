#pragma once

#include "buffer_pool_data.h"

#include <span>
#include <string>

namespace celeritas
{
    class buffer_guard
    {
    public:
        using class_type = buffer_guard;
        using source_type = std::span<const char>;

        buffer_guard(buffer_pool_data buffer_data, size_t effective_size);

        ~buffer_guard() noexcept;

        buffer_guard(const buffer_guard& rhs) = delete;

        buffer_guard& operator=(const buffer_guard& rhs) = delete;

        buffer_guard(buffer_guard&& rhs) noexcept;

        buffer_guard& operator=(buffer_guard&& rhs) noexcept;

        [[nodiscard]] char* get();

        [[nodiscard]] const char* get() const;

        [[nodiscard]] size_t size() const;

        [[nodiscard]] size_t get_effective_size() const;

        void set_effective_size(size_t size);

        [[nodiscard]] char* get(size_t offset);

        void set(const std::string& response);

        void set(const source_type& source);

        [[nodiscard]] bool is_effective() const noexcept;

    private:
        void release() noexcept;

        void do_release();

        buffer_pool_data buffer_data_;
        size_t effective_size_ = 0;
    };
}