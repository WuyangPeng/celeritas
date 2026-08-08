// 创建时间：2026-08-05
// 修改时间：2026-08-05
// 审核时间：2026-08-05

#pragma once

#include "feedback_admin_list_item.h"

#include <vector>

namespace celeritas
{
    class feedback_admin_list_result
    {
    public:
        using container_type = std::vector<feedback_admin_list_item>;

        feedback_admin_list_result() noexcept = default;

        feedback_admin_list_result(int64_t total, container_type feedbacks, int64_t bug_count, int64_t suggestion_count, int64_t report_count);

        [[nodiscard]] int64_t get_total() const noexcept;

        void set_total(int64_t total) noexcept;

        [[nodiscard]] const container_type& get_feedbacks() const noexcept;

        [[nodiscard]] container_type& get_feedbacks() noexcept;

        void set_feedbacks(container_type feedbacks);

        [[nodiscard]] int64_t get_bug_count() const noexcept;

        void set_bug_count(int64_t bug_count) noexcept;

        [[nodiscard]] int64_t get_suggestion_count() const noexcept;

        void set_suggestion_count(int64_t suggestion_count) noexcept;

        [[nodiscard]] int64_t get_report_count() const noexcept;

        void set_report_count(int64_t report_count) noexcept;

    private:
        int64_t total_ = 0;
        container_type feedbacks_;
        int64_t bug_count_ = 0;
        int64_t suggestion_count_ = 0;
        int64_t report_count_ = 0;
    };
}
