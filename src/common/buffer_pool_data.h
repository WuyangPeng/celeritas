#pragma once

#include <memory>

namespace celeritas
{
    // 使用一个结构体来保存缓冲区及其大小
    class buffer_pool_data
    {
    public:
        using class_type = buffer_pool_data;
        using data_shared_ptr = std::shared_ptr<char[]>;

        buffer_pool_data(data_shared_ptr data, size_t size);

        [[nodiscard]] data_shared_ptr& data();
        [[nodiscard]] size_t size() const;

    private:
        data_shared_ptr data_;
        size_t size_{};
    };
}