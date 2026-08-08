#pragma once

#include "database/basic/database_data_type_traits.h"

namespace celeritas
{
    class soldier_data
    {
    public:
        using class_type = soldier_data;
        using document_type = traits::document_type;

        soldier_data();

        [[nodiscard]] document_type to_document_type() const;

        [[nodiscard]] static soldier_data from_document(const document_type& document);
    };
}
