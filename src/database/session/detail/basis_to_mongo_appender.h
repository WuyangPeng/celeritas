#pragma once

#include "database/basic/basis_database.h"
#include "database/basic/database_data_type.h"

#include <bsoncxx/builder/basic/document.hpp>

#include <functional>
#include <map>

namespace celeritas
{
    class basis_to_mongo_appender
    {
    public:
        using class_type = basis_to_mongo_appender;
        using document_type = bsoncxx::builder::basic::document;
        using document_shared_ptr = std::shared_ptr<document_type>;
        using append_function = std::function<void(document_type&, const basis_database&)>;
        using container_type = std::map<database_data_type, append_function>;

        basis_to_mongo_appender();

        [[nodiscard]] static const container_type& get_appender();

        void append_document(const basis_database& database);

        [[nodiscard]] document_shared_ptr get_document() const;

    private:
        template <database_data_type T>
        static void append_basic_type(document_type& document, const basis_database& basis_database);

        template <database_data_type T>
        static void append_array_document(document_type& document, const basis_database& basis_database);

        static void append_document_item(document_type& document, const basis_database& basis_database);

        static void append_document_array_item(document_type& document, const basis_database& basis_database);

        static void append_byte_array(document_type& document, const basis_database& basis_database);

        document_shared_ptr document_;
    };
}
