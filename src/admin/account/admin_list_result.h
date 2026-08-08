// 创建时间：2026-08-04
// 修改时间：2026-08-04
// 审核时间：2026-08-07

#pragma once

#include "admin_list_item.h"

#include <vector>

namespace celeritas
{
    class admin_list_result
    {
    public:
        using container_type = std::vector<admin_list_item>;

        admin_list_result() noexcept = default;

        admin_list_result(int64_t total, container_type admins);

        [[nodiscard]] int64_t get_total() const noexcept;

        void set_total(int64_t total) noexcept;

        [[nodiscard]] const container_type& get_admins() const noexcept;

        [[nodiscard]] container_type& get_admins() noexcept;

        void set_admins(container_type list);

    private:
        int64_t total_ = 0;
        container_type admins_;
    };
}
