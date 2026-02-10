#pragma once

#include <boost/json.hpp>

#include <cstdint>
#include <string>

namespace celeritas
{
    class player_role
    {
    public:
        using class_type = player_role;
        using json_value = boost::json::value;

        player_role() noexcept = default;

        player_role(std::string role_surname, std::string role_name, bool modify_name, int64_t last_login_time);

        [[nodiscard]] std::string get_role_name() const;

        void set_role_name(const std::string& role_name);

        [[nodiscard]] int64_t get_last_login_time() const;

        void set_last_login_time(int64_t last_login_time);

        [[nodiscard]] std::string get_role_surname() const;

        void set_role_surname(const std::string& roleSurname);

        [[nodiscard]] bool is_modify_name() const;

        void set_modify_name(bool modifyName);

        static constexpr std::string_view role_surname_description = "role_surname_name";
        static constexpr std::string_view role_name_description = "role_name";
        static constexpr std::string_view modify_name_description = "modify_name";
        static constexpr std::string_view last_login_time_description = "last_login_time";

    private:
        std::string role_surname_;
        std::string role_name_;
        bool modify_name_ = false;
        int64_t last_login_time_ = 0;
    };

    using player_role_tag = boost::json::value_to_tag<player_role>;

    [[nodiscard]] player_role tag_invoke(player_role_tag, const player_role::json_value& value);

    void tag_invoke(boost::json::value_from_tag, player_role::json_value& value, const player_role& role);
}
