// 创建时间：2026-07-30
// 修改时间：2026-07-30
// 审核时间：2026-08-04

#pragma once

#include "admin/core/detail/admin_parameter.h"
#include "message/basic/http_response.h"

#include <string>

namespace celeritas
{
    class admin_update_password_parameter final : public admin_parameter
    {
    public:
        using class_type = admin_update_password_parameter;
        using base_type = admin_parameter;

        explicit admin_update_password_parameter(const http_handle_parameter_shared_ptr& http_handle_parameter);

        [[nodiscard]] bool is_failure() const override;

        [[nodiscard]] http_response get_response() const;

        [[nodiscard]] int64_t get_target_id() const noexcept;

        [[nodiscard]] const std::string& get_password() const noexcept;

        [[nodiscard]] const std::string& get_token() const noexcept;

    protected:
        void init_required_parameter() override;

        void verify_sign() override;

    private:
        optional_http_response response_;
        int64_t target_id_ = 0;
        std::string password_;
        std::string token_;
    };
}
