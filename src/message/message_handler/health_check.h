#pragma once

#include <string>

namespace celeritas
{
    class health_check
    {
    public:
        using class_type = health_check;

        health_check(std::string instance_id, bool is_healthy);

        [[nodiscard]] std::string get_instance_id() const;

        [[nodiscard]] bool is_healthy() const;

        [[nodiscard]] std::string to_json_string() const;

        [[nodiscard]] static health_check from_json_string(const std::string& json_string);

    private:
        std::string instance_id_;
        bool is_healthy_ = false;
    };
}