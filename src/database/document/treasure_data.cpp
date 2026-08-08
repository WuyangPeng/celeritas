#include "treasure_data.h"
#include "database/basic/basis_database.tpp"

celeritas::treasure_data::treasure_data()
{
}

celeritas::treasure_data::document_type celeritas::treasure_data::to_document_type() const
{
    return document_type{};
}

celeritas::treasure_data celeritas::treasure_data::from_document(const document_type& document)
{
    return treasure_data{};
}
