#pragma once

#include "service_registry/health_check_level_type.h"

#include <string>

namespace celeritas
{
    class health_check
    {
    public:
        using class_type = health_check;

        health_check(std::string instance_id, health_check_level_type health_check_level);

        [[nodiscard]] std::string get_instance_id() const;

        [[nodiscard]] health_check_level_type get_health_check_level() const;

        [[nodiscard]] std::string to_json_string() const;

        [[nodiscard]] static health_check from_json_string(const std::string& json_string);

    private:
        std::string instance_id_;
        health_check_level_type health_check_level_;
    };
}