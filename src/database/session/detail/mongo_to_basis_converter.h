#pragma once

#include "database/basic/basis_database.h"

#include <bsoncxx/array/view.hpp>
#include <bsoncxx/document/element.hpp>

#include <functional>
#include <map>

namespace celeritas
{
    class mongo_to_basis_converter
    {
    public:
        using class_type = mongo_to_basis_converter;
        using document_element_type = bsoncxx::document::element;
        using get_basis_from_field_function = std::function<basis_database(const document_element_type&)>;
        using container_type = std::map<database_data_type, get_basis_from_field_function>;

        [[nodiscard]] static const container_type& get_basis_from_field();

        [[nodiscard]] static basis_database get_string_basis(const document_element_type& row_view);

        [[nodiscard]] static basis_database get_int32_basis(const document_element_type& row_view);

        [[nodiscard]] static basis_database get_int64_basis(const document_element_type& row_view);

        [[nodiscard]] static basis_database get_double_basis(const document_element_type& row_view);

        [[nodiscard]] static basis_database get_bool_basis(const document_element_type& row_view);

        [[nodiscard]] static basis_database get_string_array_basis(const document_element_type& row_view);

        [[nodiscard]] static basis_database get_int32_array_basis(const document_element_type& row_view);

        [[nodiscard]] static basis_database get_int64_array_basis(const document_element_type& row_view);

        [[nodiscard]] static basis_database get_double_array_basis(const document_element_type& row_view);

        [[nodiscard]] static basis_database get_byte_array_basis(const document_element_type& row_view);

        [[nodiscard]] static basis_database get_document_basis_database(const document_element_type& row_view);

        [[nodiscard]] static basis_database get_document_array_basis_database(const document_element_type& row_view);

    private:
        using array_type = bsoncxx::array::view;

        template <typename T>
        [[nodiscard]] static std::vector<T> get_numeric_array(const array_type& array_view);
    };
}
