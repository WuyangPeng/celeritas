#pragma once

#include <cstdint>
#include <string>

namespace celeritas
{
    class item_flow_log_param final
    {
    public:
        using class_type = item_flow_log_param;

        item_flow_log_param(std::string action,
                            int64_t item_id,
                            int32_t template_id,
                            int64_t count_change,
                            int64_t before_count,
                            int64_t after_count,
                            std::string source_id);

        ~item_flow_log_param() = default;

        [[nodiscard]] const std::string& get_action() const noexcept;

        [[nodiscard]] int64_t get_item_id() const noexcept;

        [[nodiscard]] int32_t get_template_id() const noexcept;

        [[nodiscard]] int64_t get_count_change() const noexcept;

        [[nodiscard]] int64_t get_before_count() const noexcept;

        [[nodiscard]] int64_t get_after_count() const noexcept;

        [[nodiscard]] const std::string& get_source_id() const noexcept;

    private:
        std::string action_;
        int64_t item_id_;
        int32_t template_id_;
        int64_t count_change_;
        int64_t before_count_;
        int64_t after_count_;
        std::string source_id_;
    };
}
