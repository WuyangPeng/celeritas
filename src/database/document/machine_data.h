#pragma once

#include "database/basic/database_data_type_traits.h"

namespace celeritas
{
    class machine_data
    {
    public:
        using class_type = machine_data;
        using document_type = traits::document_type;

        machine_data();

        [[nodiscard]] document_type to_document_type() const;

        [[nodiscard]] static machine_data from_document(const document_type& document);
    };
}
