#pragma once

#include "database/basic/database_data_type_traits.h"

namespace celeritas
{
    class gift_box_data
    {
    public:
        using class_type = gift_box_data;
        using document_type = traits::document_type;

        gift_box_data();

        [[nodiscard]] document_type to_document_type() const;

        [[nodiscard]] static gift_box_data from_document(const document_type& document);
    };
}
