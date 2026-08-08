#pragma once

#include "database/basic/database_data_type_traits.h"

namespace celeritas
{
    class skill_book_data
    {
    public:
        using class_type = skill_book_data;
        using document_type = traits::document_type;

        skill_book_data();

        [[nodiscard]] document_type to_document_type() const;

        [[nodiscard]] static skill_book_data from_document(const document_type& document);
    };
}
