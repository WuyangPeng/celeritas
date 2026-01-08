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

    private:
        using array_type = bsoncxx::array::view;
        using document_array = basis_database::document_array;

        [[nodiscard]] static basis_database get_basis_database(const document_element_type& row_view);

        template <typename T>
        [[nodiscard]] static std::vector<T> get_numeric_array(const array_type& array_view);

        static void append_document(document_type& document, const basis_database& basis_database);

        template <database_data_type T>
        static void append_basic_type(document_type& document, const basis_database& basis_database);

        template <database_data_type T>
        static void append_array_document(document_type& document, const basis_database& basis_database);

        template <typename T>
        [[nodiscard]] static auto get_array_from_view(const array_type& row_view_array);

        [[nodiscard]] static document_array get_document_array_from_view(const array_type& row_view_array);

        [[nodiscard]] static basis_database get_document_basis_database(const document_element_type& row_view);

        [[nodiscard]] static basis_database get_document_array_basis_database(const document_element_type& row_view);
        
        [[nodiscard]] static basis_database get_array_basis_database_from_view(const document_element_type& row_view);

        static void append_document_item(document_type& document, const basis_database& basis_database);

        static void append_document_array_item(document_type& document, const basis_database& basis_database);
    };
}
