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
    class announcement_admin_list_parameter final : public admin_parameter
    {
    public:
        using class_type = announcement_admin_list_parameter;
        using base_type = admin_parameter;
        using optional_int32 = std::optional<int32_t>;
        using optional_announcement_status = std::optional<announcement_status>;

        explicit announcement_admin_list_parameter(const http_handle_parameter_shared_ptr& http_handle_parameter);

        [[nodiscard]] bool is_failure() const override;

        [[nodiscard]] http_response get_response() const;

        [[nodiscard]] int get_page() const noexcept;

        [[nodiscard]] int get_page_size() const noexcept;

        [[nodiscard]] const std::string& get_keyword() const noexcept;

        [[nodiscard]] const optional_int32& get_tag() const noexcept;

        [[nodiscard]] const optional_announcement_status& get_status() const noexcept;

        [[nodiscard]] const std::string& get_token() const noexcept;

    protected:
        void init_required_parameter() override;

        void init_optional_parameter() override;

        void verify_sign() override;

    private:
        optional_http_response response_;
        int page_ = 0;
        int page_size_ = 0;
        std::string keyword_;
        optional_int32 tag_;
        optional_announcement_status status_;
        std::string token_;
    };
}
