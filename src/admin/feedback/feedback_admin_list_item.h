// 创建时间：2026-08-05
// 修改时间：2026-08-05
// 审核时间：2026-08-05

#pragma once

#include <boost/json.hpp>
#include <string>

namespace celeritas
{
    class feedback_admin_list_item
    {
    public:
        using class_type = feedback_admin_list_item;
        using json_value = boost::json::value;

        feedback_admin_list_item() noexcept = default;

        feedback_admin_list_item(int64_t id,
                                 int64_t app_id,
                                 int32_t type,
                                 std::string content,
                                 std::string device_info,
                                 std::string server,
                                 std::string player_id,
                                 bool is_anonymous,
                                 std::string image_data,
                                 int64_t create_time);

        [[nodiscard]] int64_t get_id() const noexcept;

        [[nodiscard]] int64_t get_app_id() const noexcept;

        [[nodiscard]] int32_t get_type() const noexcept;

        [[nodiscard]] const std::string& get_content() const noexcept;

        [[nodiscard]] const std::string& get_device_info() const noexcept;

        [[nodiscard]] const std::string& get_server() const noexcept;

        [[nodiscard]] const std::string& get_player_id() const noexcept;

        [[nodiscard]] bool get_is_anonymous() const noexcept;

        [[nodiscard]] const std::string& get_image_data() const noexcept;

        [[nodiscard]] int64_t get_create_time() const noexcept;

        static constexpr std::string_view id_description = "id";
        static constexpr std::string_view app_id_description = "app_id";
        static constexpr std::string_view type_description = "type";
        static constexpr std::string_view content_description = "content";
        static constexpr std::string_view device_info_description = "device_info";
        static constexpr std::string_view server_description = "server";
        static constexpr std::string_view player_id_description = "player_id";
        static constexpr std::string_view is_anonymous_description = "is_anonymous";
        static constexpr std::string_view image_data_description = "image_data";
        static constexpr std::string_view create_time_description = "create_time";

    private:
        int64_t id_ = 0;
        int64_t app_id_ = 0;
        int32_t type_ = 0;
        std::string content_;
        std::string device_info_;
        std::string server_;
        std::string player_id_;
        bool is_anonymous_ = false;
        std::string image_data_;
        int64_t create_time_ = 0;
    };

    using feedback_admin_list_item_tag = boost::json::value_to_tag<feedback_admin_list_item>;

    [[nodiscard]] feedback_admin_list_item tag_invoke(feedback_admin_list_item_tag, const feedback_admin_list_item::json_value& value);

    void tag_invoke(boost::json::value_from_tag, feedback_admin_list_item::json_value& value, const feedback_admin_list_item& item);
}
