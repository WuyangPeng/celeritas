// 创建时间：2026-08-06
// 修改时间：2026-08-06
// 审核时间：2026-08-08

#pragma once

#include "announcement_type.h"

#include <boost/json.hpp>
#include <string>

namespace celeritas
{
    class announcement_admin_list_item
    {
    public:
        using class_type = announcement_admin_list_item;
        using json_value = boost::json::value;

        announcement_admin_list_item() noexcept = default;

        announcement_admin_list_item(int64_t id,
                                     int64_t app_id,
                                     std::string title,
                                     int32_t tag,
                                     std::string banner_url,
                                     std::string content,
                                     int32_t priority,
                                     int64_t publish_time,
                                     announcement_status status,
                                     std::string server,
                                     int64_t create_time,
                                     int64_t update_time);

        [[nodiscard]] int64_t get_id() const noexcept;

        [[nodiscard]] int64_t get_app_id() const noexcept;

        [[nodiscard]] const std::string& get_title() const noexcept;

        [[nodiscard]] int32_t get_tag() const noexcept;

        [[nodiscard]] const std::string& get_banner_url() const noexcept;

        [[nodiscard]] const std::string& get_content() const noexcept;

        [[nodiscard]] int32_t get_priority() const noexcept;

        [[nodiscard]] int64_t get_publish_time() const noexcept;

        [[nodiscard]] announcement_status get_status() const noexcept;

        [[nodiscard]] const std::string& get_server() const noexcept;

        [[nodiscard]] int64_t get_create_time() const noexcept;

        [[nodiscard]] int64_t get_update_time() const noexcept;

        static constexpr std::string_view id_description = "id";
        static constexpr std::string_view app_id_description = "app_id";
        static constexpr std::string_view title_description = "title";
        static constexpr std::string_view tag_description = "tag";
        static constexpr std::string_view banner_url_description = "banner_url";
        static constexpr std::string_view content_description = "content";
        static constexpr std::string_view priority_description = "priority";
        static constexpr std::string_view publish_time_description = "publish_time";
        static constexpr std::string_view status_description = "status";
        static constexpr std::string_view server_description = "server";
        static constexpr std::string_view create_time_description = "create_time";
        static constexpr std::string_view update_time_description = "update_time";

    private:
        int64_t id_ = 0;
        int64_t app_id_ = 0;
        std::string title_;
        int32_t tag_ = 0;
        std::string banner_url_;
        std::string content_;
        int32_t priority_ = 0;
        int64_t publish_time_ = 0;
        announcement_status status_ = announcement_status::draft;
        std::string server_;
        int64_t create_time_ = 0;
        int64_t update_time_ = 0;
    };

    using announcement_admin_list_item_tag = boost::json::value_to_tag<announcement_admin_list_item>;

    [[nodiscard]] announcement_admin_list_item tag_invoke(announcement_admin_list_item_tag, const announcement_admin_list_item::json_value& value);

    void tag_invoke(boost::json::value_from_tag, announcement_admin_list_item::json_value& value, const announcement_admin_list_item& item);
}
