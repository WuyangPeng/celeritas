#pragma once

#include "entity_attribute.h"

#include <boost/json.hpp>

#include <vector>

namespace celeritas
{
    class database_attribute
    {
    public:
        using class_type = database_attribute;
        using entity_type = std::vector<entity_attribute>;

        database_attribute() noexcept = default;

        [[nodiscard]] std::string get_class_name() const;

        [[nodiscard]] std::string get_database_name() const;

        [[nodiscard]] entity_type get_entity() const;

        void set_class_name(const std::string& class_name);

        void set_database_name(const std::string& database_name);

        void set_entity(const entity_type& entity);

    private:
        std::string class_name_;
        std::string database_name_;
        entity_type entity_;
    };

    [[nodiscard]] database_attribute tag_invoke(boost::json::value_to_tag<database_attribute>, boost::json::value const& value);
}