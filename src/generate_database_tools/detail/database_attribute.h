#pragma once

#include "entity_attribute.h"

#include <vector>

namespace celeritas
{
    class database_attribute
    {
    public:
        using class_type = database_attribute;
        using entity_type = std::vector<entity_attribute>;

        database_attribute(std::string class_name, std::string database_name, entity_type entity);

        [[nodiscard]] std::string get_class_name() const;

        [[nodiscard]] std::string get_database_name() const;

        [[nodiscard]] entity_type get_entity() const;

    private:
        std::string class_name_;
        std::string database_name_;
        entity_type entity_;
    };
}