#include "database_attribute_container.h"
#include "common/celeritas_error.h"

celeritas::database_attribute_container::database_attribute_container(container container)
    : container_{ std::move(container) }
{
}

celeritas::database_attribute_container celeritas::tag_invoke(boost::json::value_to_tag<database_attribute_container>, boost::json::value const& value)
{
    if (!value.is_array())
    {
        throw celeritas_error("Root element is not a JSON array. Expecting an array to preserve order.");
    }

    database_attribute_container container{ std::move(boost::json::value_to<std::vector<database_attribute> >(value)) };

    return container;
}