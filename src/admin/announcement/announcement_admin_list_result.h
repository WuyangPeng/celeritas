// 创建时间：2026-08-06
// 修改时间：2026-08-06
// 审核时间：2026-08-08

#pragma once

#include "announcement_admin_list_item.h"

#include <vector>

namespace celeritas
{
    class announcement_admin_list_result
    {
    public:
        using container_type = std::vector<announcement_admin_list_item>;

        announcement_admin_list_result() noexcept = default;

        announcement_admin_list_result(int64_t total, container_type announcements);

        [[nodiscard]] int64_t get_total() const noexcept;

        void set_total(int64_t total) noexcept;

        [[nodiscard]] const container_type& get_announcements() const noexcept;

        [[nodiscard]] container_type& get_announcements() noexcept;

        void set_announcements(container_type announcements);

    private:
        int64_t total_ = 0;
        container_type announcements_;
    };
}
