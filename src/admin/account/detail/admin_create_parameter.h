// 创建时间：2026-07-25
// 修改时间：2026-07-31
// 审核时间：2026-07-31

#pragma once

#include "admin/admin_fwd.h"
#include "admin/core/detail/admin_parameter.h"
#include "message/basic/http_response.h"

#include <string>

namespace celeritas
{
    class admin_create_parameter final : public admin_parameter
    {
    public:
        using class_type = admin_create_parameter;
        using base_type = admin_parameter;

        explicit admin_create_parameter(const http_handle_parameter_shared_ptr& http_handle_parameter);

        [[nodiscard]] bool is_failure() const override;

        [[nodiscard]] http_response get_response() const;

        [[nodiscard]] const std::string& get_username() const noexcept;

        [[nodiscard]] const std::string& get_password() const noexcept;

        [[nodiscard]] admin_role_type get_role() const noexcept;

        [[nodiscard]] const std::string& get_nickname() const noexcept;

        [[nodiscard]] const std::string& get_avatar() const noexcept;

        [[nodiscard]] const std::string& get_token() const noexcept;

    protected:
        void init_required_parameter() override;

        void init_optional_parameter() override;

        void verify_sign() override;

    private:
        optional_http_response response_;
        std::string username_;
        std::string password_;
        admin_role_type role_;
        std::string nickname_;
        std::string avatar_;
        std::string token_;
    };
}
