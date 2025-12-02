#include "basis_database_container.h"
#include "common/celeritas_error.h"

#include <boost/numeric/conversion/cast.hpp>

#include <algorithm>

celeritas::basis_database_container::basis_database_container(const basis_database& basisDatabase)
    : container_{ basisDatabase }
{
}

celeritas::basis_database_container::basis_database_container(object_container container)
    : container_{ std::move(container) }
{
}

void celeritas::basis_database_container::modify(const basis_database& basis_database)
{
    const auto result = std::ranges::remove_if(container_, [&basis_database](const auto& element) {
        return element.get_field_name() == basis_database.get_field_name();
    });

    container_.erase(result.begin(), result.end());

    container_.emplace_back(basis_database);
}

void celeritas::basis_database_container::set(const object_container& container)
{
    container_ = container;
}

void celeritas::basis_database_container::clear()
{
    container_.clear();
}

std::any celeritas::basis_database_container::get_any_value(std::string_view field_name) const
{
    if (const auto result = std::ranges::find_if(container_, [&field_name](const auto& element) {
            return element.get_field_name() == field_name;
        });
        result != container_.cend())
    {
        return result->get_any_value();
    }

    throw celeritas_error{ "field name does not exist" };
}

celeritas::basis_database_container::object_container_const_iter celeritas::basis_database_container::begin() const noexcept
{
    return container_.cbegin();
}

celeritas::basis_database_container::object_container_const_iter celeritas::basis_database_container::end() const noexcept
{
    return container_.cend();
}

int celeritas::basis_database_container::get_size() const
{
    return boost::numeric_cast<int>(container_.size());
}
