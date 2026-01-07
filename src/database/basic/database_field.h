#pragma once

#include "database_index_type.h"
#include "database/database_fwd.h"

namespace celeritas
{
    class database_field
    {
    public:
        using class_type = database_field;

        constexpr database_field(const std::string_view field_name, const database_data_type data_type, const database_index_type index_type = database_index_type::null)
            : field_name_{ field_name }, data_type_{ data_type }, index_type_{ index_type }
        {
        }

        [[nodiscard]] constexpr std::string_view get_field_name() const noexcept
        {
            return field_name_;
        }

        [[nodiscard]] constexpr database_data_type get_data_type() const noexcept
        {
            return data_type_;
        }

        [[nodiscard]] constexpr database_index_type get_index_type() const noexcept
        {
            return index_type_;
        }

    private:
        std::string_view field_name_;
        database_data_type data_type_;
        database_index_type index_type_;
    };
}