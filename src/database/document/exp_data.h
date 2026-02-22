#pragma once

#include "database/basic/database_data_type_traits.h"

namespace celeritas
{
    class exp_data
    {
    public:
        using class_type = exp_data;
        using document_type = traits::document_type;

        exp_data();

        [[nodiscard]] document_type to_document_type() const;

        [[nodiscard]] static exp_data from_document(const document_type& document);
    };
}
