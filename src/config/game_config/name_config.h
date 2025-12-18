#pragma once

#include "config/config_fwd.h"

#include <string>

namespace celeritas
{
    class name_config
    {
    public:
        using class_type = name_config;

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