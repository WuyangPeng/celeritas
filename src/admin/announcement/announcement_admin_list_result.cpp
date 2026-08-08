// 创建时间：2026-08-06
// 修改时间：2026-08-06
// 审核时间：2026-08-08

#include "announcement_admin_list_result.h"

celeritas::announcement_admin_list_result::announcement_admin_list_result(const int64_t total, container_type announcements)
    : total_{ total }, announcements_{ std::move(announcements) }
{
}

int64_t celeritas::announcement_admin_list_result::get_total() const noexcept
{
    return total_;
}

void celeritas::announcement_admin_list_result::set_total(const int64_t total) noexcept
{
    total_ = total;
}

const celeritas::announcement_admin_list_result::container_type& celeritas::announcement_admin_list_result::get_announcements() const noexcept
{
    return announcements_;
}

celeritas::announcement_admin_list_result::container_type& celeritas::announcement_admin_list_result::get_announcements() noexcept
{
    return announcements_;
}

void celeritas::announcement_admin_list_result::set_announcements(container_type announcements)
{
    announcements_ = std::move(announcements);
}
