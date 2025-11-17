#pragma once

#include "database_attribute.h"

#include <boost/json.hpp>

namespace celeritas
{
    class database_attribute_container
    {
    public:
        using class_type = database_attribute_container;
        using container = std::vector<database_attribute>;
        using container_const_iterator = container::const_iterator;

        explicit database_attribute_container(container container);

        [[nodiscard]] container_const_iterator begin() const;

        [[nodiscard]] container_const_iterator end() const;

    private:
        container container_;
    };

    [[nodiscard]] database_attribute_container tag_invoke(boost::json::value_to_tag<database_attribute_container>, boost::json::value const& value);
}