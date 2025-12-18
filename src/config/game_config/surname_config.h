#pragma once

#include <string>

namespace celeritas
{
    class surname_config
    {
    public:
        using class_type = surname_config;

        surname_config(int id, std::string name, int weight);

        [[nodiscard]] int get_id() const;

        [[nodiscard]] std::string get_name() const;

        [[nodiscard]] int get_weight() const;

    private:
        int id_;
        std::string name_;
        int weight_;
    };
}