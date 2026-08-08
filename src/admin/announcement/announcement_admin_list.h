// 创建时间：2026-08-06
// 修改时间：2026-08-06
// 审核时间：2026-08-08

#pragma once

#include "announcement_admin_list_result.h"
#include "admin/core/admin_service_base.h"

#include <optional>
#include <string>

namespace celeritas
{
    class announcement_admin_list final : public admin_service_base
    {
    public:
        using class_type = announcement_admin_list;
        using base_type = admin_service_base;

        explicit announcement_admin_list(http_handle_parameter_shared_ptr handle_parameter);

        [[nodiscard]] void_awaitable_type response() override;

        [[nodiscard]] void_awaitable_type send_error_response() override;

    private:
        using container_type = announcement_admin_list_result::container_type;
        using announcement_admin_list_result_awaitable = boost::asio::awaitable<announcement_admin_list_result>;
        using optional_int32 = std::optional<int32_t>;
        using optional_announcement_status = std::optional<announcement_status>;

        [[nodiscard]] static announcement_admin_list_result_awaitable query_announcements(const database_pool_shared_ptr& mysql_pool,
                                                                                          int64_t app_id,
                                                                                          int page,
                                                                                          int page_size,
                                                                                          const std::string& keyword,
                                                                                          const optional_int32& tag,
                                                                                          const optional_announcement_status& status);
    };
}
