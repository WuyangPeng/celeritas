// 创建时间：2026-08-06
// 修改时间：2026-08-06
// 审核时间：2026-08-06

#pragma once

#include "admin/core/detail/admin_parameter.h"
#include "message/basic/http_response.h"

#include <string>

namespace celeritas
{
    class announcement_admin_delete_parameter final : public admin_parameter
    {
    public:
        using class_type = announcement_admin_delete_parameter;
        using base_type = admin_parameter;

        explicit announcement_admin_delete_parameter(const http_handle_parameter_shared_ptr& http_handle_parameter);

        [[nodiscard]] bool is_failure() const override;

        [[nodiscard]] http_response get_response() const;

        [[nodiscard]] int64_t get_id() const noexcept;

        [[nodiscard]] const std::string& get_token() const noexcept;

    protected:
        void init_required_parameter() override;

        void verify_sign() override;

    private:
        optional_http_response response_;
        int64_t id_ = 0;
        std::string token_;
    };
}
