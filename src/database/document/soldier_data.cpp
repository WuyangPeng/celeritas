#include "soldier_data.h"
#include "database/basic/basis_database.tpp"

celeritas::soldier_data::soldier_data()
{
}

celeritas::soldier_data::document_type celeritas::soldier_data::to_document_type() const
{
    return document_type{};
}

celeritas::soldier_data celeritas::soldier_data::from_document(const document_type& document)
{
    return soldier_data{};
}
