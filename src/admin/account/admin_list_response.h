// 创建时间：2026-07-30
// 修改时间：2026-07-30
// 审核时间：2026-08-07

#pragma once

#include "admin_list_item.h"
#include "message/basic/http_response.h"

#include <vector>

namespace celeritas
{
    class admin_list_response final : public http_response
    {
    public:
        using class_type = admin_list_response;
        using bass_type = http_response;
        using container_type = std::vector<admin_list_item>;

        admin_list_response() noexcept = default;

        explicit admin_list_response(game_error_type code);

        admin_list_response(game_error_type code, std::string message);

        admin_list_response(game_error_type code, std::string message, int64_t total, container_type admins);

        admin_list_response(bass_type http_response, int64_t total, container_type admins);

        explicit admin_list_response(bass_type http_response);

        [[nodiscard]] int64_t get_total() const noexcept;

        void set_total(int64_t total);

        [[nodiscard]] const container_type& get_admins() const noexcept;

        void set_admins(container_type admins);

        [[nodiscard]] std::string to_json_string() const override;

        [[nodiscard]] static admin_list_response from_json_string(const std::string& json_string);

        [[nodiscard]] static admin_list_response tag_invoke(const json_value& value);

        static constexpr std::string_view total_description = "total";
        static constexpr std::string_view admins_description = "admins";

    private:
        [[nodiscard]] static admin_list_response do_from_json_string(const std::string& json_string);

        int64_t total_ = 0;
        container_type admins_;
    };

    using admin_list_response_tag = boost::json::value_to_tag<admin_list_response>;

    [[nodiscard]] admin_list_response tag_invoke(admin_list_response_tag, const http_response::json_value& value);

    void tag_invoke(boost::json::value_from_tag tag, admin_list_response::json_value& value, const admin_list_response& admin_list_response);
}
