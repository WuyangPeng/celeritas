// 创建时间：2026-07-26
// 修改时间：2026-07-31
// 审核时间：2026-07-31

#pragma once

#include "admin/admin_fwd.h"
#include "admin/account/detail/admin_account_internal_fwd.h"
#include "admin/core/admin_service_base.h"

namespace celeritas
{
    class admin_create final : public admin_service_base
    {
    public:
        using class_type = admin_create;
        using base_type = admin_service_base;

        explicit admin_create(http_handle_parameter_shared_ptr handle_parameter);

        [[nodiscard]] void_awaitable_type response() override;

        [[nodiscard]] void_awaitable_type send_error_response() override;

    private:
        [[nodiscard]] static bool_awaitable_type check_username_exists(const database_pool_shared_ptr& mysql_pool,
                                                                       const std::string& username,
                                                                       int64_t app_id);

        [[nodiscard]] static bool_awaitable_type create_and_save_account(const database_pool_shared_ptr& mysql_pool,
                                                                         const admin_create_parameter& parameter,
                                                                         int64_t account_id,
                                                                         int64_t creator_id);

        [[nodiscard]] static bool check_permission(admin_role_type operator_role, admin_role_type target_role);
    };
}
