// 创建时间：2026-08-06
// 修改时间：2026-08-06
// 审核时间：2026-08-08

#pragma once

#include "announcement_admin_list_item.h"
#include "message/basic/http_response.h"

#include <vector>

namespace celeritas
{
    class announcement_admin_list_response final : public http_response
    {
    public:
        using class_type = announcement_admin_list_response;
        using bass_type = http_response;
        using container_type = std::vector<announcement_admin_list_item>;

        announcement_admin_list_response() noexcept = default;

        explicit announcement_admin_list_response(game_error_type code);

        announcement_admin_list_response(game_error_type code, std::string message);

        announcement_admin_list_response(game_error_type code, std::string message, int64_t total, container_type announcements);

        announcement_admin_list_response(bass_type http_response, int64_t total, container_type announcements);

        explicit announcement_admin_list_response(bass_type http_response);

        [[nodiscard]] int64_t get_total() const noexcept;

        void set_total(int64_t total);

        [[nodiscard]] const container_type& get_announcements() const noexcept;

        void set_announcements(container_type announcements);

        [[nodiscard]] std::string to_json_string() const override;

        [[nodiscard]] static announcement_admin_list_response from_json_string(const std::string& json_string);

        [[nodiscard]] static announcement_admin_list_response tag_invoke(const json_value& value);

        static constexpr std::string_view total_description = "total";
        static constexpr std::string_view announcements_description = "announcements";

    private:
        [[nodiscard]] static announcement_admin_list_response do_from_json_string(const std::string& json_string);

        int64_t total_ = 0;
        container_type announcements_;
    };

    using announcement_admin_list_response_tag = boost::json::value_to_tag<announcement_admin_list_response>;

    [[nodiscard]] announcement_admin_list_response tag_invoke(announcement_admin_list_response_tag, const http_response::json_value& value);

    void tag_invoke(boost::json::value_from_tag tag, announcement_admin_list_response::json_value& value, const announcement_admin_list_response& announcement_admin_list_response);
}
