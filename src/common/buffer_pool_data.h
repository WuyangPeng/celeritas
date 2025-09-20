#pragma once

#include <memory>

namespace celeritas
{
    // 使用一个结构体来保存缓冲区及其大小
    class buffer_pool_data
    {
    public:
        using class_type = buffer_pool_data;
        using data_unique_ptr = std::unique_ptr<char[]>;

        buffer_pool_data() noexcept = default;

        buffer_pool_data(data_unique_ptr data, size_t size);

        ~buffer_pool_data() noexcept = default;

        buffer_pool_data(const buffer_pool_data& rhs) = delete;

        buffer_pool_data& operator=(const buffer_pool_data& rhs) = delete;

        buffer_pool_data(buffer_pool_data&& rhs) noexcept;

        buffer_pool_data& operator=(buffer_pool_data&& rhs) noexcept;

        [[nodiscard]] char* data() noexcept;

        [[nodiscard]] size_t size() const noexcept;

        [[nodiscard]] bool is_effective() const noexcept;

    private:
        data_unique_ptr data_;
        size_t size_{};
    };
}