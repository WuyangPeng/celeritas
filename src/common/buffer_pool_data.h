#pragma once

#include <span>
#include <string>
#include <vector>

namespace celeritas
{
    // 使用一个结构体来保存缓冲区及其大小
    class buffer_pool_data
    {
    public:
        using class_type = buffer_pool_data;
        using source_type = std::span<const char>;

        explicit buffer_pool_data(size_t size);

        [[nodiscard]] char* data();

        [[nodiscard]] const char* data() const;

        [[nodiscard]] size_t size() const noexcept;

        [[nodiscard]] bool is_effective() const noexcept;

        [[nodiscard]] char* get(size_t offset);

        void set(const std::string& response);

        void set(const source_type& source);

    private:
        using data_container = std::vector<char>;

        data_container data_;
    };
}