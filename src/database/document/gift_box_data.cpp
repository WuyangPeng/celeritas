#include "gift_box_data.h"
#include "database/basic/basis_database.tpp"

celeritas::gift_box_data::gift_box_data()
{
}

celeritas::gift_box_data::document_type celeritas::gift_box_data::to_document_type() const
{
    return document_type{};
}

celeritas::gift_box_data celeritas::gift_box_data::from_document(const document_type& document)
{
    return gift_box_data{};
}
