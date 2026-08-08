// 创建时间：2026-07-30
// 修改时间：2026-07-30
// 审核时间：2026-08-07

#pragma once

#include "admin/admin_fwd.h"
#include "admin/core/admin_service_base.h"

namespace celeritas
{
    class admin_login final : public admin_service_base
    {
    public:
        using class_type = admin_login;
        using base_type = admin_service_base;

        explicit admin_login(http_handle_parameter_shared_ptr handle_parameter);

        [[nodiscard]] void_awaitable_type response() override;

        [[nodiscard]] void_awaitable_type send_error_response() override;

    private:
        using optional_string = std::optional<std::string>;
        using optional_string_awaitable = boost::asio::awaitable<optional_string>;

        [[nodiscard]] static optional_admin_account_awaitable load_account(const database_pool_shared_ptr& mysql_pool,
                                                                           const std::string& username,
                                                                           int64_t app_id);

        [[nodiscard]] static bool_awaitable_type update_login_info(const database_pool_shared_ptr& mysql_pool,
                                                                   admin_account& account,
                                                                   const std::string& client_ip);

        [[nodiscard]] static optional_string_awaitable create_session(int64_t account_id);
    };
}
