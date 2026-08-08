// 创建时间：2026-07-30
// 修改时间：2026-07-30
// 审核时间：2026-08-04

#pragma once

#include "admin/core/detail/admin_parameter.h"
#include "message/basic/http_response.h"

#include <string>

namespace celeritas
{
    class admin_login_parameter final : public admin_parameter
    {
    public:
        using class_type = admin_login_parameter;
        using base_type = admin_parameter;

        explicit admin_login_parameter(const http_handle_parameter_shared_ptr& http_handle_parameter);

        [[nodiscard]] bool is_failure() const override;

        [[nodiscard]] http_response get_response() const;

        [[nodiscard]] const std::string& get_username() const noexcept;

        [[nodiscard]] const std::string& get_password() const noexcept;

    protected:
        void init_required_parameter() override;

        void verify_sign() override;

    private:
        optional_http_response response_;
        std::string username_;
        std::string password_;
    };
}
