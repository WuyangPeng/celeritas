// 创建时间：2026-08-04
// 修改时间：2026-08-04
// 审核时间：2026-08-07

#include "admin_list_result.h"

celeritas::admin_list_result::admin_list_result(const int64_t total, container_type admins)
    : total_{ total }, admins_{ std::move(admins) }
{
}

int64_t celeritas::admin_list_result::get_total() const noexcept
{
    return total_;
}

void celeritas::admin_list_result::set_total(const int64_t total) noexcept
{
    total_ = total;
}

const celeritas::admin_list_result::container_type& celeritas::admin_list_result::get_admins() const noexcept
{
    return admins_;
}

celeritas::admin_list_result::container_type& celeritas::admin_list_result::get_admins() noexcept
{
    return admins_;
}

void celeritas::admin_list_result::set_admins(container_type list)
{
    admins_ = std::move(list);
}
