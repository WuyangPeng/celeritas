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

        player_time_refresh(time_refresh_type time_refresh_type, int64_t parameter, player_component_type player_component_type);

        [[nodiscard]] time_refresh_type get_time_refresh_type() const;

        void set_time_refresh_type(time_refresh_type timeRefreshType);

        [[nodiscard]] int64_t get_parameter() const;

        void set_parameter(int64_t parameter);

        [[nodiscard]] component_container get_component() const;

        void set_component(const component_container& component);

        void add_component(player_component_type component);

        void remove_component(player_component_type playerComponent);

        [[nodiscard]] int64_t get_last_refresh_time() const;

        void set_last_refresh_time(int64_t last_refresh_time);

        [[nodiscard]] bool is_can_refresh() const;

        [[nodiscard]] std::string to_json_string() const;

        [[nodiscard]] static player_time_refresh from_json_string(const std::string& json_string);

        static constexpr std::string_view time_refresh_type_description = "time_refresh_type";
        static constexpr std::string_view parameter_description = "parameter";
        static constexpr std::string_view component_description = "component";
        static constexpr std::string_view last_refresh_time_description = "last_refresh_time";

    private:
        time_refresh_type time_refresh_type_;
        int64_t parameter_;
        component_container component_;
        int64_t last_refresh_time_;
    };
}