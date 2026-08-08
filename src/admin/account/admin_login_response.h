// 创建时间：2026-07-30
// 修改时间：2026-07-30
// 审核时间：2026-08-07

#pragma once

#include "message/basic/http_response.h"

#include <string>

namespace celeritas
{
    class admin_login_response final : public http_response
    {
    public:
        using class_type = admin_login_response;
        using bass_type = http_response;

        admin_login_response() noexcept = default;

        explicit admin_login_response(game_error_type code);

        admin_login_response(game_error_type code, std::string message);

        admin_login_response(game_error_type code,
                             std::string message,
                             std::string token,
                             std::string account_id,
                             std::string username,
                             int32_t role,
                             std::string avatar);

        admin_login_response(bass_type http_response,
                             std::string token,
                             std::string account_id,
                             std::string username,
                             int32_t role,
                             std::string avatar);

        explicit admin_login_response(bass_type http_response);

        [[nodiscard]] const std::string& get_token() const noexcept;

        void set_token(const std::string& token);

        [[nodiscard]] const std::string& get_account_id() const noexcept;

        void set_account_id(const std::string& account_id);

        [[nodiscard]] const std::string& get_username() const noexcept;

        void set_username(const std::string& username);

        [[nodiscard]] int32_t get_role() const noexcept;

        void set_role(int32_t role);

        [[nodiscard]] const std::string& get_avatar() const noexcept;

        void set_avatar(const std::string& avatar);

        [[nodiscard]] std::string to_json_string() const override;

        [[nodiscard]] static admin_login_response from_json_string(const std::string& json_string);

        [[nodiscard]] static admin_login_response tag_invoke(const json_value& value);

        static constexpr std::string_view token_description = "token";
        static constexpr std::string_view account_id_description = "account_id";
        static constexpr std::string_view username_description = "username";
        static constexpr std::string_view role_description = "role";
        static constexpr std::string_view avatar_description = "avatar";

    private:
        [[nodiscard]] static admin_login_response do_from_json_string(const std::string& json_string);

        std::string token_;
        std::string account_id_;
        std::string username_;
        int32_t role_ = 0;
        std::string avatar_;
    };

    using admin_login_response_tag = boost::json::value_to_tag<admin_login_response>;

    [[nodiscard]] admin_login_response tag_invoke(admin_login_response_tag, const http_response::json_value& value);

    void tag_invoke(boost::json::value_from_tag tag, admin_login_response::json_value& value, const admin_login_response& admin_login_response);
}
