// 创建时间：2026-08-06
// 修改时间：2026-08-06
// 审核时间：2026-08-06

#pragma once

#include "admin/admin_fwd.h"
#include "admin/core/detail/admin_parameter.h"
#include "message/basic/http_response.h"

#include <optional>
#include <string>

namespace celeritas
{
    class announcement_admin_update_parameter final : public admin_parameter
    {
    public:
        using class_type = announcement_admin_update_parameter;
        using base_type = admin_parameter;
        using optional_int32 = std::optional<int32_t>;
        using optional_int64 = std::optional<int64_t>;
        using optional_string = std::optional<std::string>;
        using optional_announcement_status = std::optional<announcement_status>;

        explicit announcement_admin_update_parameter(const http_handle_parameter_shared_ptr& http_handle_parameter);

        [[nodiscard]] bool is_failure() const override;

        [[nodiscard]] http_response get_response() const;

        [[nodiscard]] int64_t get_id() const noexcept;

        [[nodiscard]] const optional_string& get_title() const noexcept;

        [[nodiscard]] const optional_int32& get_tag() const noexcept;

        [[nodiscard]] const optional_string& get_banner_url() const noexcept;

        [[nodiscard]] const optional_string& get_content() const noexcept;

        [[nodiscard]] const optional_int32& get_priority() const noexcept;

        [[nodiscard]] const optional_int64& get_publish_time() const noexcept;

        [[nodiscard]] const optional_announcement_status& get_status() const noexcept;

        [[nodiscard]] const optional_string& get_server() const noexcept;

        [[nodiscard]] const std::string& get_token() const noexcept;

    protected:
        void init_required_parameter() override;

        void init_optional_parameter() override;

        void verify_sign() override;

    private:
        optional_http_response response_;
        int64_t id_ = 0;
        optional_string title_;
        optional_int32 tag_;
        optional_string banner_url_;
        optional_string content_;
        optional_int32 priority_;
        optional_int64 publish_time_;
        optional_announcement_status status_;
        optional_string server_;
        std::string token_;
    };
}
