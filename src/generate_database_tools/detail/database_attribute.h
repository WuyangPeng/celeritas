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
        using entity_const_iterator = std::vector<entity_attribute>::const_iterator;

        database_attribute();

        [[nodiscard]] std::string get_class_name() const;

        [[nodiscard]] std::string get_database_name() const;

        [[nodiscard]] entity_type get_entity() const;

        [[nodiscard]] std::string get_key_type() const;

        [[nodiscard]] std::string get_key_name() const;

        [[nodiscard]] entity_const_iterator begin() const;

        [[nodiscard]] entity_const_iterator end() const;

        void set_class_name(const std::string& class_name);

        void set_database_name(const std::string& database_name);

        void set_entity(const entity_type& entity);

        [[nodiscard]] int size() const;

    private:
        std::string class_name_;
        std::string database_name_;
        entity_type entity_;
        std::string key_type_;
        std::string key_name_;
    };

    [[nodiscard]] database_attribute tag_invoke(boost::json::value_to_tag<database_attribute>, boost::json::value const& value);
}