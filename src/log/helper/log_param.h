#pragma once

#include <cstdint>
#include <string>

namespace celeritas
{
    class log_param
    {
    public:
        using class_type = log_param;

        log_param(int64_t player_id, int64_t timestamp, std::string ip_address);

        [[nodiscard]] int64_t get_player_id() const noexcept;

        [[nodiscard]] int64_t get_timestamp() const noexcept;

        [[nodiscard]] const std::string& get_ip_address() const noexcept;

    private:
        int64_t player_id_;
        int64_t timestamp_;
        std::string ip_address_;
    };
}
