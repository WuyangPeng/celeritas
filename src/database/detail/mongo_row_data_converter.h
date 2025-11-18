#pragma once

#include "database/basis_database.h"
#include "database/database_field.h"

#include <bsoncxx/document/element.hpp>

#include <vector>

namespace celeritas
{
    class mongo_row_data_converter
    {
    public:
        using class_type = mongo_row_data_converter;
        using document_element_type = bsoncxx::document::element;
        using database_field_container = std::vector<database_field>;

        [[nodiscard]] static basis_database get_basis_database(const database_field_container& field_name_container, const document_element_type& row_view);
    };
}
