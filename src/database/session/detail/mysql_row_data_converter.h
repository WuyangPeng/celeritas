#pragma once

#include "database/basic/basis_database.h"

#include <boost/mysql.hpp>

#include <functional>
#include <map>

namespace celeritas
{
    class mysql_row_data_converter
    {
    public:
        using class_type = mysql_row_data_converter;
        using field_view_type = boost::mysql::field_view;
        using convert_function = std::function<basis_database(const database_field&, const field_view_type&)>;
        using container_type = std::map<database_data_type, convert_function>;

        [[nodiscard]] static basis_database get_basis_database(const database_field& field_name, const field_view_type& row_view);

    private:
        [[nodiscard]] static const container_type& get_converters();

        [[nodiscard]] static basis_database convert_string(const database_field& field_name, const field_view_type& row_view);

        [[nodiscard]] static basis_database convert_document(const database_field& field_name, const field_view_type& row_view);

        [[nodiscard]] static basis_database do_convert_document(const database_field& field_name, const std::string& value);

        [[nodiscard]] static basis_database convert_int32(const database_field& field_name, const field_view_type& row_view);

        [[nodiscard]] static basis_database convert_int64(const database_field& field_name, const field_view_type& row_view);

        [[nodiscard]] static basis_database convert_double(const database_field& field_name, const field_view_type& row_view);

        [[nodiscard]] static basis_database convert_bool(const database_field& field_name, const field_view_type& row_view);

        [[nodiscard]] static basis_database convert_string_array(const database_field& field_name, const field_view_type& row_view);

        [[nodiscard]] static basis_database convert_document_array(const database_field& field_name, const field_view_type& row_view);

        [[nodiscard]] static basis_database do_convert_document_array(const database_field& field_name, const std::string& value);

        [[nodiscard]] static basis_database convert_int32_array(const database_field& field_name, const field_view_type& row_view);

        [[nodiscard]] static basis_database convert_int64_array(const database_field& field_name, const field_view_type& row_view);

        [[nodiscard]] static basis_database convert_double_array(const database_field& field_name, const field_view_type& row_view);

        [[nodiscard]] static basis_database convert_byte_array(const database_field& field_name, const field_view_type& row_view);

        [[nodiscard]] static basis_database convert_default(const database_field& field_name, const field_view_type& row_view);

        template <typename ArrayType>
        [[nodiscard]] static basis_database to_numeric_array_basis(const database_field& field_name, const field_view_type& row_view);
    };
}
