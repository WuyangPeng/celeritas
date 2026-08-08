// 创建时间：2026-07-30
// 修改时间：2026-07-30
// 审核时间：2026-08-07

#pragma once

#include "admin_list_result.h"
#include "admin/core/admin_service_base.h"

#include <vector>

namespace celeritas
{
    class admin_list final : public admin_service_base
    {
    public:
        using class_type = admin_list;
        using base_type = admin_service_base;

        explicit admin_list(http_handle_parameter_shared_ptr handle_parameter);

        [[nodiscard]] void_awaitable_type response() override;

        [[nodiscard]] void_awaitable_type send_error_response() override;

    private:
        using container_type = admin_list_result::container_type;
        using admin_list_result_awaitable = boost::asio::awaitable<admin_list_result>;

        [[nodiscard]] static admin_list_result_awaitable query_accounts(const database_pool_shared_ptr& mysql_pool,
                                                                        int64_t app_id,
                                                                        int page,
                                                                        int page_size);
    };
}
