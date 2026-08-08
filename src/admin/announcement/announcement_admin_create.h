// 创建时间：2026-08-06
// 修改时间：2026-08-06
// 审核时间：2026-08-06

#pragma once

#include "admin/core/admin_service_base.h"

namespace celeritas
{
    class announcement_admin_create final : public admin_service_base
    {
    public:
        using class_type = announcement_admin_create;
        using base_type = admin_service_base;

        explicit announcement_admin_create(http_handle_parameter_shared_ptr handle_parameter);

        [[nodiscard]] void_awaitable_type response() override;

        [[nodiscard]] void_awaitable_type send_error_response() override;
    };
}
