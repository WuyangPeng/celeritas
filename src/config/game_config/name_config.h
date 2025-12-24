#pragma once

#include "config/luban/generated/schema.h"

#include <string>

namespace celeritas
{
    class name_config
    {
    public:
        using class_type = name_config;
        using sex_type = config::sex_type;

        name_config(int id, std::string name, sex_type sex_type, int weight);

        [[nodiscard]] int get_id() const;

        [[nodiscard]] std::string get_name() const;

        [[nodiscard]] sex_type get_sex_type() const;

        [[nodiscard]] int get_weight() const;

    private:
        int id_;
        std::string name_;
        sex_type sex_type_;
        int weight_;
    };
}