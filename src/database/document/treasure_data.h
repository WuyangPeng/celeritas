#pragma once

#include "database/basic/database_data_type_traits.h"

namespace celeritas
{
    class treasure_data
    {
    public:
        using class_type = treasure_data;
        using document_type = traits::document_type;

        treasure_data();

        [[nodiscard]] document_type to_document_type() const;

        [[nodiscard]] static treasure_data from_document(const document_type& document);
    };
}
