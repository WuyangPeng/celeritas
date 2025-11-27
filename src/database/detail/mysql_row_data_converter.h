#pragma once

#include "database/basis_database.h"

#include <boost/mysql.hpp>

namespace celeritas
{
    class mysql_row_data_converter
    {
    public:
        using class_type = mysql_row_data_converter;
        using field_view_type = boost::mysql::field_view;

        [[nodiscard]] static basis_database get_basis_database(const database_field& field_name, const field_view_type& row_view);

        template <typename ArrayType>
        [[nodiscard]] static basis_database to_numeric_array_basis(const database_field& field_name, const field_view_type& row_view);
    };
}