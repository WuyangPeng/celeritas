#pragma once

#include "database/basic/database_data_type_traits.h"

namespace celeritas
{
    class hero_data
    {
    public:
        using class_type = hero_data;
        using document_type = traits::document_type;

        hero_data() = default;

        [[nodiscard]] document_type to_document_type() const;

        [[nodiscard]] static hero_data from_document(const document_type& document);
    };
}
