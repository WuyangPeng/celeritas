#pragma once

#include "database/document/player_time_refresh.h"
#include "player/player_fwd.h"

#include <cstdint>

namespace celeritas
{
    class player_time_refresh_key
    {
    public:
        using class_type = player_time_refresh_key;

        player_time_refresh_key(time_refresh_type time_refresh_type, int64_t parameter1, int64_t parameter2);

        explicit player_time_refresh_key(const player_time_refresh& player_time_refresh);

        [[nodiscard]] time_refresh_type get_time_refresh_type() const;

        [[nodiscard]] int64_t get_parameter1() const;

        [[nodiscard]] int64_t get_parameter2() const;

    private:
        time_refresh_type time_refresh_type_;
        int64_t parameter1_;
        int64_t parameter2_;
    };

    [[nodiscard]] bool operator<(const player_time_refresh_key& lhs, const player_time_refresh_key& rhs);
}