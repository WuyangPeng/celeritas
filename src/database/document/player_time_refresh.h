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

        [[nodiscard]] int64_t get_time_refresh_id() const;

        void set_time_refresh_id(int64_t timeRefreshId);

        [[nodiscard]] int64_t get_last_refresh_time() const;

        void set_last_refresh_time(int64_t lastRefreshTime);

        [[nodiscard]] std::string to_json_string() const;

        [[nodiscard]] static player_time_refresh from_json_string(const std::string& json_string);

        static constexpr std::string_view time_refresh_id_description = "time_refresh_id";
        static constexpr std::string_view last_refresh_time_description = "last_refresh_time";

    private:
        int64_t time_refresh_id_;
        int64_t last_refresh_time_;
    };
}