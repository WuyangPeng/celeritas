// 创建时间：2026-07-30
// 修改时间：2026-07-30
// 审核时间：2026-08-07

#pragma once

#include "admin_status_type.h"

#include <boost/json.hpp>

#include <string>

namespace celeritas
{
    class admin_list_item
    {
    public:
        using class_type = admin_list_item;
        using json_value = boost::json::value;

        admin_list_item() noexcept = default;

        admin_list_item(std::string account_id,
                        std::string username,
                        int64_t app_id,
                        int32_t role,
                        std::string nickname,
                        std::string avatar,
                        std::string creator_account_id,
                        admin_status_type status,
                        int64_t create_time,
                        int64_t update_time,
                        int64_t last_login_time,
                        std::string last_login_ip);

        [[nodiscard]] const std::string& get_account_id() const noexcept;

        [[nodiscard]] const std::string& get_username() const noexcept;

        [[nodiscard]] int64_t get_app_id() const noexcept;

        [[nodiscard]] int32_t get_role() const noexcept;

        [[nodiscard]] const std::string& get_nickname() const noexcept;

        [[nodiscard]] const std::string& get_avatar() const noexcept;

        [[nodiscard]] const std::string& get_creator_account_id() const noexcept;

        [[nodiscard]] admin_status_type get_status() const noexcept;

        [[nodiscard]] int64_t get_create_time() const noexcept;

        [[nodiscard]] int64_t get_update_time() const noexcept;

        [[nodiscard]] int64_t get_last_login_time() const noexcept;

        [[nodiscard]] const std::string& get_last_login_ip() const noexcept;

        static constexpr std::string_view account_id_description = "account_id";
        static constexpr std::string_view username_description = "username";
        static constexpr std::string_view app_id_description = "app_id";
        static constexpr std::string_view role_description = "role";
        static constexpr std::string_view nickname_description = "nickname";
        static constexpr std::string_view avatar_description = "avatar";
        static constexpr std::string_view creator_account_id_description = "creator_account_id";
        static constexpr std::string_view status_description = "status";
        static constexpr std::string_view create_time_description = "create_time";
        static constexpr std::string_view update_time_description = "update_time";
        static constexpr std::string_view last_login_time_description = "last_login_time";
        static constexpr std::string_view last_login_ip_description = "last_login_ip";

    private:
        std::string account_id_;
        std::string username_;
        int64_t app_id_ = 0;
        int32_t role_ = 0;
        std::string nickname_;
        std::string avatar_;
        std::string creator_account_id_;
        admin_status_type status_ = admin_status_type::enabled;
        int64_t create_time_ = 0;
        int64_t update_time_ = 0;
        int64_t last_login_time_ = 0;
        std::string last_login_ip_;
    };

    using admin_list_item_tag = boost::json::value_to_tag<admin_list_item>;

    [[nodiscard]] admin_list_item tag_invoke(admin_list_item_tag, const admin_list_item::json_value& value);

    void tag_invoke(boost::json::value_from_tag, admin_list_item::json_value& value, const admin_list_item& item);
}
