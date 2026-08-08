#include "blueprint_data.h"
#include "database/basic/basis_database.tpp"

celeritas::blueprint_data::blueprint_data()
{
}

celeritas::blueprint_data::document_type celeritas::blueprint_data::to_document_type() const
{
    return document_type{};
}

celeritas::blueprint_data celeritas::blueprint_data::from_document(const document_type& document)
{
    return blueprint_data{};
}
