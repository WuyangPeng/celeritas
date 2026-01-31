#include "frame_data.h"
#include "database/basic/basis_database.tpp"

celeritas::frame_data::frame_data()
{
}

celeritas::frame_data::document_type celeritas::frame_data::to_document_type() const
{
    return document_type{};
}

celeritas::frame_data celeritas::frame_data::from_document(const document_type& document)
{
    return frame_data{};
}
