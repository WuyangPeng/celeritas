// 创建时间：2026-08-06
// 修改时间：2026-08-06
// 审核时间：2026-08-06

#pragma once

#include "message/basic/http_response.h"

namespace celeritas
{
    class announcement_admin_delete_response final : public http_response
    {
    public:
        using class_type = announcement_admin_delete_response;
        using bass_type = http_response;

        announcement_admin_delete_response() noexcept = default;

        explicit announcement_admin_delete_response(game_error_type code);

        announcement_admin_delete_response(game_error_type code, std::string message);

        explicit announcement_admin_delete_response(bass_type http_response);

        [[nodiscard]] std::string to_json_string() const override;

        [[nodiscard]] static announcement_admin_delete_response from_json_string(const std::string& json_string);

        [[nodiscard]] static announcement_admin_delete_response tag_invoke(const json_value& value);

    private:
        [[nodiscard]] static announcement_admin_delete_response do_from_json_string(const std::string& json_string);
    };

    using announcement_admin_delete_response_tag = boost::json::value_to_tag<announcement_admin_delete_response>;

    [[nodiscard]] announcement_admin_delete_response tag_invoke(announcement_admin_delete_response_tag, const http_response::json_value& value);

    void tag_invoke(boost::json::value_from_tag tag, announcement_admin_delete_response::json_value& value, const announcement_admin_delete_response& announcement_admin_delete_response);
}
