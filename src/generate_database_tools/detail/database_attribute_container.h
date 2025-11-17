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

        explicit database_attribute_container(container container);

    private:
        container container_;
    };

    [[nodiscard]] database_attribute_container tag_invoke(boost::json::value_to_tag<database_attribute_container>, boost::json::value const& value);
}