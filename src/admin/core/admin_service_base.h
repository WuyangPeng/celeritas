// 创建时间：2026-07-28
// 修改时间：2026-07-28
// 审核时间：2026-07-28

#pragma once

#include "database/database_fwd.h"
#include "database/generated/mysql/admin/admin_account.h"
#include "message/basic/http_service_base.h"

#include <optional>

namespace celeritas
{
    class admin_service_base : public http_service_base
    {
    public:
        using class_type = admin_service_base;
        using base_type = http_service_base;
        using base_type::base_type;

    protected:
        using database_pool_shared_ptr = std::shared_ptr<database_pool_base>;
        using bool_awaitable_type = boost::asio::awaitable<bool>;
        using optional_admin_account = std::optional<admin_account>;
        using optional_admin_account_awaitable = boost::asio::awaitable<optional_admin_account>;
        using optional_int64 = std::optional<int64_t>;
        using optional_int64_awaitable = boost::asio::awaitable<optional_int64>;

        [[nodiscard]] static optional_admin_account_awaitable get_admin_account(const database_pool_shared_ptr& mysql_pool, int64_t target_id);

        [[nodiscard]] static optional_int64_awaitable resolve_operator_id(const std::string& token);
    };
}
