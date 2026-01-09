#pragma once

#include "database/basic/basis_database.h"

#include <bsoncxx/types.hpp>

#include <functional>
#include <map>

namespace celeritas
{
    class mongo_element_to_basis_converter
    {
    public:
        using document_element_type = bsoncxx::document::element;
        using get_basis_from_element_function = std::function<basis_database(const document_element_type&)>;
        using container_type = std::map<bsoncxx::type, get_basis_from_element_function>;

        [[nodiscard]] static const container_type& get_element_converters();

    private:
        [[nodiscard]] static basis_database get_double_from_element(const document_element_type& row_view);

        [[nodiscard]] static basis_database get_string_from_element(const document_element_type& row_view);

        [[nodiscard]] static basis_database get_bool_from_element(const document_element_type& row_view);

        [[nodiscard]] static basis_database get_document_from_element(const document_element_type& row_view);

        [[nodiscard]] static basis_database get_int32_from_element(const document_element_type& row_view);

        [[nodiscard]] static basis_database get_int64_from_element(const document_element_type& row_view);

        [[nodiscard]] static basis_database get_array_basis_database_from_view(const document_element_type& row_view);
    };
}
