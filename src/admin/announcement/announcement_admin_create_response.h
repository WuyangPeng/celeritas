// 创建时间：2026-08-06
// 修改时间：2026-08-06
// 审核时间：2026-08-06

#pragma once

#include "announcement_type.h"
#include "message/basic/http_response.h"

#include <string>

namespace celeritas
{
    class announcement_admin_create_response final : public http_response
    {
    public:
        using class_type = announcement_admin_create_response;
        using bass_type = http_response;

        announcement_admin_create_response() noexcept = default;

        explicit announcement_admin_create_response(game_error_type code);

        announcement_admin_create_response(game_error_type code, std::string message);

        announcement_admin_create_response(game_error_type code,
                                           std::string message,
                                           int64_t id,
                                           std::string title,
                                           announcement_status status);

        announcement_admin_create_response(bass_type http_response,
                                           int64_t id,
                                           std::string title,
                                           announcement_status status);

        explicit announcement_admin_create_response(bass_type http_response);

        [[nodiscard]] int64_t get_id() const noexcept;

        void set_id(int64_t id) noexcept;

        [[nodiscard]] const std::string& get_title() const noexcept;

        void set_title(std::string title);

        [[nodiscard]] announcement_status get_status() const noexcept;

        void set_status(announcement_status status);

        [[nodiscard]] std::string to_json_string() const override;

        [[nodiscard]] static announcement_admin_create_response from_json_string(const std::string& json_string);

        [[nodiscard]] static announcement_admin_create_response tag_invoke(const json_value& value);

        static constexpr std::string_view id_description = "id";
        static constexpr std::string_view title_description = "title";
        static constexpr std::string_view status_description = "status";

    private:
        [[nodiscard]] static announcement_admin_create_response do_from_json_string(const std::string& json_string);

        int64_t id_ = 0;
        std::string title_;
        announcement_status status_ = announcement_status::draft;
    };

    using announcement_admin_create_response_tag = boost::json::value_to_tag<announcement_admin_create_response>;

    [[nodiscard]] announcement_admin_create_response tag_invoke(announcement_admin_create_response_tag, const http_response::json_value& value);

    void tag_invoke(boost::json::value_from_tag tag, announcement_admin_create_response::json_value& value, const announcement_admin_create_response& announcement_admin_create_response);
}
