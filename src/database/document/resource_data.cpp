#include "resource_data.h"
#include "database/basic/basis_database.tpp"

celeritas::resource_data::resource_data()
{
}

celeritas::resource_data::document_type celeritas::resource_data::to_document_type() const
{
    return document_type{};
}

celeritas::resource_data celeritas::resource_data::from_document(const document_type& document)
{
    return resource_data{};
}
