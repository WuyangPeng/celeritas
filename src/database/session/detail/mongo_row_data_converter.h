#pragma once

#include "database/basic/basis_database.h"
#include "database/basic/database_field.h"

#include <bsoncxx/builder/basic/document.hpp>
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
        using document_type = bsoncxx::builder::basic::document;
        using basis_database_container_const_shared_ptr = std::shared_ptr<const basis_database_container>;

        [[nodiscard]] static basis_database get_basis_database(const database_field_container& field_name_container, const document_element_type& row_view);

        [[nodiscard]] static document_type get_document(const basis_database_container_const_shared_ptr& container);

        [[nodiscard]] static basis_database get_basis_database(const document_element_type& row_view);
    };
}
