#pragma once

#include "database/basic/database_data_type_traits.h"

namespace celeritas
{
    class avatar_data
    {
    public:
        using class_type = avatar_data;
        using document_type = traits::document_type;

        avatar_data();

        [[nodiscard]] document_type to_document_type() const;

        [[nodiscard]] static avatar_data from_document(const document_type& document);
    };
}
