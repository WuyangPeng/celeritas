#pragma once

#include <cstdint>
#include <mutex>

namespace celeritas
{
    class snowflake_generator
    {
    public:
        using class_type = snowflake_generator;

        [[nodiscard]] static snowflake_generator& get_instance() noexcept;

        /**
         * @brief 生成一个全局唯一的雪花ID.
         * @param datacenter_id 数据中心/区域ID.
         * @param worker_id 工作节点ID.
         * @return 64位整型的唯一ID.
         */
        [[nodiscard]] int64_t generate(int datacenter_id, int worker_id);

    private:
        snowflake_generator() noexcept;

        [[nodiscard]] static int64_t til_next_millis(int64_t last_timestamp);

        [[nodiscard]] int64_t handle_clock_backwards(int64_t timestamp) const;

        void update_timestamp_and_sequence(int64_t timestamp);

        std::mutex id_mutex_;
        int64_t last_timestamp_ = -1;
        int64_t sequence_ = 0;
    };
}
