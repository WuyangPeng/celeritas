#pragma once

#include "player/player_fwd.h"

#include <cstdint>
#include <string>

namespace celeritas
{
    class player_time_refresh
    {
    public:
        using class_type = player_time_refresh;

        player_time_refresh() noexcept;

        [[nodiscard]] time_refresh_type get_time_refresh() const;

        [[nodiscard]] int get_parameter() const;

        [[nodiscard]] int64_t get_last_refresh_time() const;

        void set_time_refresh(time_refresh_type timeRefresh);

        void set_parameter(int parameter);

        void set_last_refresh_time(int64_t lastRefreshTime);

        [[nodiscard]] std::string to_json_string() const;

        [[nodiscard]] static player_time_refresh from_json_string(const std::string& json_string);

        static constexpr std::string_view time_refresh_description = "time_refresh";
        static constexpr std::string_view parameter_description = "parameter";
        static constexpr std::string_view last_refresh_time_description = "last_refresh_time";

    private:
        time_refresh_type time_refresh_;
        int parameter_;
        int64_t last_refresh_time_;
    };
}