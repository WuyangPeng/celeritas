#pragma once

#include "database/basic/database_data_type_traits.h"

namespace celeritas
{
    class resource_data
    {
    public:
        using class_type = resource_data;
        using document_type = traits::document_type;

        resource_data();

        [[nodiscard]] document_type to_document_type() const;

        [[nodiscard]] static resource_data from_document(const document_type& document);
    };
}
