#pragma once

#include "database/basic/database_data_type_traits.h"

namespace celeritas
{
    class frame_data
    {
    public:
        using class_type = frame_data;
        using document_type = traits::document_type;

        frame_data();

        [[nodiscard]] document_type to_document_type() const;

        [[nodiscard]] static frame_data from_document(const document_type& document);
    };
}
