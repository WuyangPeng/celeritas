// 创建时间：2026-08-05
// 修改时间：2026-08-05
// 审核时间：2026-08-05

#pragma once

#include "feedback_admin_list_result.h"
#include "admin/core/admin_service_base.h"

#include <vector>

namespace celeritas
{
    class feedback_admin_list final : public admin_service_base
    {
    public:
        using class_type = feedback_admin_list;
        using base_type = admin_service_base;

        explicit feedback_admin_list(http_handle_parameter_shared_ptr handle_parameter);

        [[nodiscard]] void_awaitable_type response() override;

        [[nodiscard]] void_awaitable_type send_error_response() override;

    private:
        using container_type = feedback_admin_list_result::container_type;
        using feedback_admin_list_result_awaitable = boost::asio::awaitable<feedback_admin_list_result>;

        [[nodiscard]] static feedback_admin_list_result_awaitable query_feedbacks(const database_pool_shared_ptr& mysql_pool,
                                                                                  int64_t app_id,
                                                                                  int page,
                                                                                  int page_size);
    };
}
