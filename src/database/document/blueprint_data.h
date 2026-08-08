#pragma once

#include "database/basic/database_data_type_traits.h"

namespace celeritas
{
    class blueprint_data
    {
    public:
        using class_type = blueprint_data;
        using document_type = traits::document_type;

        blueprint_data();

        [[nodiscard]] document_type to_document_type() const;

        [[nodiscard]] static blueprint_data from_document(const document_type& document);
    };
}
