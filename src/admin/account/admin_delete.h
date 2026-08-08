// 创建时间：2026-07-27
// 修改时间：2026-07-28
// 审核时间：2026-07-28

#pragma once

#include "admin/admin_fwd.h"
#include "admin/core/admin_service_base.h"

namespace celeritas
{
    class admin_delete final : public admin_service_base
    {
    public:
        using class_type = admin_delete;
        using base_type = admin_service_base;

        explicit admin_delete(http_handle_parameter_shared_ptr handle_parameter);

        [[nodiscard]] void_awaitable_type response() override;

        [[nodiscard]] void_awaitable_type send_error_response() override;

    private:
        [[nodiscard]] static optional_admin_account_awaitable load_target_account(const database_pool_shared_ptr& mysql_pool,
                                                                                  int64_t target_id,
                                                                                  int64_t app_id);

        [[nodiscard]] static bool_awaitable_type delete_account(const database_pool_shared_ptr& mysql_pool,
                                                                const admin_account& account);

        [[nodiscard]] static bool check_permission(admin_role_type operator_role,
                                                   admin_role_type target_role,
                                                   int64_t operator_account_id,
                                                   int64_t creator_account_id);
    };
}
