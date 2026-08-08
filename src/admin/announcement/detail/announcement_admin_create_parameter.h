// 创建时间：2026-08-06
// 修改时间：2026-08-06
// 审核时间：2026-08-06

#pragma once

#include "admin/admin_fwd.h"
#include "admin/core/detail/admin_parameter.h"
#include "message/basic/http_response.h"

#include <string>

namespace celeritas
{
    class announcement_admin_create_parameter final : public admin_parameter
    {
    public:
        using class_type = announcement_admin_create_parameter;
        using base_type = admin_parameter;

        explicit announcement_admin_create_parameter(const http_handle_parameter_shared_ptr& http_handle_parameter);

        [[nodiscard]] bool is_failure() const override;

        [[nodiscard]] http_response get_response() const;

        [[nodiscard]] const std::string& get_title() const noexcept;

        [[nodiscard]] int get_tag() const noexcept;

        [[nodiscard]] const std::string& get_banner_url() const noexcept;

        [[nodiscard]] const std::string& get_content() const noexcept;

        [[nodiscard]] int get_priority() const noexcept;

        [[nodiscard]] int64_t get_publish_time() const noexcept;

        [[nodiscard]] announcement_status get_status() const noexcept;

        [[nodiscard]] const std::string& get_server() const noexcept;

        [[nodiscard]] const std::string& get_token() const noexcept;

    protected:
        void init_required_parameter() override;

        void init_optional_parameter() override;

        void verify_sign() override;

    private:
        optional_http_response response_;
        std::string title_;
        int tag_ = 0;
        std::string banner_url_;
        std::string content_;
        int priority_ = 0;
        int64_t publish_time_ = 0;
        announcement_status status_;
        std::string server_ = "all";
        std::string token_;
    };
}
