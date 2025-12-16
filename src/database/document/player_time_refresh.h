#pragma once

#include "player/player_fwd.h"

#include <cstdint>
#include <string>
#include <vector>

namespace celeritas
{
    class player_time_refresh
    {
    public:
        using class_type = player_time_refresh;
        using component_container = std::vector<player_component_type>;

        player_time_refresh() noexcept;

        player_time_refresh(time_refresh_type time_refresh_type, int64_t parameter1, int64_t parameter2, player_component_type player_component_type);

        [[nodiscard]] time_refresh_type get_time_refresh_type() const;

        void set_time_refresh_type(time_refresh_type timeRefreshType);

        [[nodiscard]] int64_t get_parameter1() const;

        void set_parameter1(int64_t parameter);

        [[nodiscard]] int64_t get_parameter2() const;

        void set_parameter2(int64_t parameter2);

        [[nodiscard]] component_container get_component() const;

        void set_component(const component_container& component);

        void add_component(player_component_type component);

        void remove_component(player_component_type playerComponent);

        [[nodiscard]] int64_t get_last_refresh_time() const;

        void set_last_refresh_time(int64_t last_refresh_time);

        [[nodiscard]] bool is_can_refresh() const;

        [[nodiscard]] int64_t get_next_refresh_time() const;

        [[nodiscard]] bool is_default() const;

        [[nodiscard]] std::string to_json_string() const;

        [[nodiscard]] static player_time_refresh from_json_string(const std::string& json_string);

        static constexpr std::string_view time_refresh_type_description = "time_refresh_type";
        static constexpr std::string_view parameter1_description = "parameter1";
        static constexpr std::string_view parameter2_description = "parameter2";
        static constexpr std::string_view component_description = "component";
        static constexpr std::string_view last_refresh_time_description = "last_refresh_time";

    private:
        time_refresh_type time_refresh_type_;
        int64_t parameter1_;
        int64_t parameter2_;
        component_container component_;
        int64_t last_refresh_time_;
    };
}