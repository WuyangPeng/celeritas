#include "machine_data.h"
#include "database/basic/basis_database.tpp"

celeritas::machine_data::machine_data()
{
}

celeritas::machine_data::document_type celeritas::machine_data::to_document_type() const
{
    return document_type{};
}

celeritas::machine_data celeritas::machine_data::from_document(const document_type& document)
{
    return machine_data{};
}
