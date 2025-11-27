#pragma once

#include "service_registry/health_check_level_type.h"

#include <boost/json.hpp>

#include <string>

namespace celeritas
{
    class health_check
    {
    public:
        using class_type = health_check;

        health_check() noexcept = default;

        health_check(std::string instance_id, health_check_level_type health_check_level);

        [[nodiscard]] std::string get_instance_id() const;

        [[nodiscard]] health_check_level_type get_health_check_level() const;

        void set_instance_id(const std::string& instance_id);

        void set_health_check_level(health_check_level_type health_check_level);

        [[nodiscard]] std::string to_json_string() const;

        [[nodiscard]] static health_check from_json_string(const std::string& json_string);

        static constexpr std::string_view instance_id_description = "instance_id";
        static constexpr std::string_view health_check_level_description = "health_check_level";

    private:
        std::string instance_id_;
        health_check_level_type health_check_level_ = health_check_level_type::health;
    };

    [[nodiscard]] health_check tag_invoke(boost::json::value_to_tag<health_check>, const boost::json::value& value);

    void tag_invoke(boost::json::value_from_tag, boost::json::value& value, const health_check& health_check);
}
