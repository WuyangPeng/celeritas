#include "basis_database_container.h"
#include "common/core/celeritas_error.h"

#include <boost/numeric/conversion/cast.hpp>

#include <algorithm>
#include <unordered_set>

celeritas::basis_database_container::basis_database_container(const basis_database& basisDatabase)
    : container_{ basisDatabase }
{
}

celeritas::basis_database_container::basis_database_container(object_container container)
    : container_{ std::move(container) }
{
    std::unordered_set<std::string> field_names{};
    for (const auto& db : container_)
    {
        if (!field_names.emplace(db.get_field_name()).second)
        {
            throw celeritas_error{ "duplicate field name in basis_database_container" };
        }
    }
}

void celeritas::basis_database_container::modify(const basis_database& basis_database)
{
    if (const auto result = std::ranges::find_if(container_, [&basis_database](const auto& element) {
            return element.get_field_name() == basis_database.get_field_name();
        });
        result != container_.cend())
    {
        *result = basis_database;
    }
    else
    {
        container_.emplace_back(basis_database);
    }
}

void celeritas::basis_database_container::clear()
{
    container_.clear();
}

const celeritas::basis_database::value_variant& celeritas::basis_database_container::get_variant_value(std::string_view field_name) const
{
    if (const auto result = std::ranges::find_if(container_, [&field_name](const auto& element) {
            return element.get_field_name() == field_name;
        });
        result != container_.cend())
    {
        return result->get_variant_value();
    }

    static constexpr value_variant default_value{};
    return default_value;
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

bool celeritas::operator==(const basis_database_container& lhs, const basis_database_container& rhs)
{
    if (lhs.get_size() == rhs.get_size())
    {
        const auto pred = [rhs](const basis_database& basis_database) {
            return basis_database.get_variant_value() == rhs.get_variant_value(basis_database.get_field_name());
        };

        if (std::ranges::all_of(lhs, pred))
        {
            return true;
        }
    }

    return false;
}

bool celeritas::operator!=(const basis_database_container& lhs, const basis_database_container& rhs)
{
    return !(lhs == rhs);
}
