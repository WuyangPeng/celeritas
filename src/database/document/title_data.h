#pragma once

#include "database/basic/database_data_type_traits.h"

namespace celeritas
{
    class title_data
    {
    public:
        using class_type = title_data;
        using document_type = traits::document_type;

        title_data() = default;

        [[nodiscard]] document_type to_document_type() const;

        [[nodiscard]] static title_data from_document(const document_type& document);
    };
}
