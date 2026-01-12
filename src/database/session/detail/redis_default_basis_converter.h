#pragma once

#include "database/basic/basis_database.h"
#include "database/basic/database_field.h"

#include <functional>
#include <map>

namespace celeritas
{
    class redis_default_basis_converter
    {
    public:
        using class_type = redis_default_basis_converter;
        using get_default_basis_function = std::function<basis_database(const database_field&)>;
        using container_type = std::map<database_data_type, get_default_basis_function>;

        [[nodiscard]] static basis_database get_default_basis_database(const database_field& field_name);

    private:
        [[nodiscard]] static const container_type& get_converters();

        [[nodiscard]] static basis_database get_string_basis(const database_field& field_name);

        [[nodiscard]] static basis_database get_document_basis(const database_field& field_name);

        [[nodiscard]] static basis_database get_int32_basis(const database_field& field_name);

        [[nodiscard]] static basis_database get_int64_basis(const database_field& field_name);

        [[nodiscard]] static basis_database get_double_basis(const database_field& field_name);

        [[nodiscard]] static basis_database get_bool_basis(const database_field& field_name);

        [[nodiscard]] static basis_database get_string_array_basis(const database_field& field_name);

        [[nodiscard]] static basis_database get_document_array_basis(const database_field& field_name);

        [[nodiscard]] static basis_database get_int32_array_basis(const database_field& field_name);

        [[nodiscard]] static basis_database get_int64_array_basis(const database_field& field_name);

        [[nodiscard]] static basis_database get_double_array_basis(const database_field& field_name);

        [[nodiscard]] static basis_database get_byte_array_basis(const database_field& field_name);

        [[nodiscard]] static basis_database get_default_basis(const database_field& field_name);
    };
}
