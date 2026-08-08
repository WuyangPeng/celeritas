#include "database_select_options.h"
#include "common/core/celeritas_error.h"

#include <algorithm>

celeritas::database_select_options& celeritas::database_select_options::add_order_by(const std::string_view field_name, const sort_order order)
{
    order_by_.emplace_back(database_order_by{ field_name, order });
    return *this;
}

celeritas::database_select_options& celeritas::database_select_options::set_limit(const int64_t limit)
{
    if (limit < 0)
    {
        throw celeritas_error{ "database_select_options limit must be non-negative" };
    }

    limit_ = limit;
    return *this;
}

celeritas::database_select_options& celeritas::database_select_options::add_like_condition(std::vector<std::string_view> field_names, std::string pattern)
{
    like_conditions_.emplace_back(database_like_condition{ std::move(field_names), std::move(pattern) });
    return *this;
}

celeritas::database_select_options& celeritas::database_select_options::set_offset(const int64_t offset)
{
    if (offset < 0)
    {
        throw celeritas_error{ "database_select_options offset must be non-negative" };
    }

    offset_ = offset;
    return *this;
}

const celeritas::database_select_options::order_by_container& celeritas::database_select_options::get_order_by() const noexcept
{
    return order_by_;
}

std::optional<int64_t> celeritas::database_select_options::get_limit() const noexcept
{
    return limit_;
}

std::optional<int64_t> celeritas::database_select_options::get_offset() const noexcept
{
    return offset_;
}

const celeritas::database_select_options::like_condition_container& celeritas::database_select_options::get_like_conditions() const noexcept
{
    return like_conditions_;
}

void celeritas::database_select_options::validate(const database_field_container& field_name_container) const
{
    if (offset_.has_value() && !limit_.has_value())
    {
        throw celeritas_error{ "database_select_options offset requires limit" };
    }

    for (const auto& order_by : order_by_)
    {
        const auto found = std::ranges::any_of(field_name_container, [&order_by](const database_field& field) {
            return field.get_field_name() == order_by.field_name;
        });

        if (!found)
        {
            throw celeritas_error{ "database_select_options order_by field '{}' is not in field container", order_by.field_name };
        }
    }
}
