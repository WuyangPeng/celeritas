// 创建时间：2026-07-26
// 修改时间：2026-07-26
// 审核时间：2026-07-26

#pragma once

#include "message/basic/http_response.h"

#include <string>

namespace celeritas
{
    class admin_create_response final : public http_response
    {
    public:
        using class_type = admin_create_response;
        using bass_type = http_response;

        admin_create_response() noexcept = default;

        explicit admin_create_response(game_error_type code);

        admin_create_response(game_error_type code, std::string message);

        admin_create_response(game_error_type code,
                              std::string message,
                              int64_t account_id,
                              std::string username);

        admin_create_response(bass_type http_response,
                              int64_t account_id,
                              std::string username);

        explicit admin_create_response(bass_type http_response);

        [[nodiscard]] int64_t get_account_id() const;

        void set_account_id(int64_t account_id);

        [[nodiscard]] std::string get_username() const;

        void set_username(const std::string& username);

        [[nodiscard]] std::string to_json_string() const override;

        [[nodiscard]] static admin_create_response from_json_string(const std::string& json_string);

        [[nodiscard]] static admin_create_response tag_invoke(const json_value& value);

        static constexpr std::string_view account_id_description = "account_id";
        static constexpr std::string_view username_description = "username";

    private:
        [[nodiscard]] static admin_create_response do_from_json_string(const std::string& json_string);

        int64_t account_id_ = 0;
        std::string username_;
    };

    using admin_create_response_tag = boost::json::value_to_tag<admin_create_response>;

    [[nodiscard]] admin_create_response tag_invoke(admin_create_response_tag, const http_response::json_value& value);

    void tag_invoke(boost::json::value_from_tag tag, admin_create_response::json_value& value, const admin_create_response& admin_create_response);
}
