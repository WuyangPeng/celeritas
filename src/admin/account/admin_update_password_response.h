// 创建时间：2026-07-30
// 修改时间：2026-07-30
// 审核时间：2026-08-07

#pragma once

#include "message/basic/http_response.h"

namespace celeritas
{
    class admin_update_password_response final : public http_response
    {
    public:
        using class_type = admin_update_password_response;
        using bass_type = http_response;

        admin_update_password_response() noexcept = default;

        explicit admin_update_password_response(game_error_type code);

        admin_update_password_response(game_error_type code, std::string message);

        explicit admin_update_password_response(bass_type http_response);

        [[nodiscard]] std::string to_json_string() const override;

        [[nodiscard]] static admin_update_password_response from_json_string(const std::string& json_string);

        [[nodiscard]] static admin_update_password_response tag_invoke(const json_value& value);

    private:
        [[nodiscard]] static admin_update_password_response do_from_json_string(const std::string& json_string);
    };

    using admin_update_password_response_tag = boost::json::value_to_tag<admin_update_password_response>;

    [[nodiscard]] admin_update_password_response tag_invoke(admin_update_password_response_tag, const http_response::json_value& value);

    void tag_invoke(boost::json::value_from_tag tag, admin_update_password_response::json_value& value, const admin_update_password_response& admin_update_password_response);
}
