#include "avatar_data.h"
#include "database/basic/basis_database.tpp"

celeritas::avatar_data::avatar_data()
{
}

celeritas::avatar_data::document_type celeritas::avatar_data::to_document_type() const
{
    return document_type{};
}

celeritas::avatar_data celeritas::avatar_data::from_document(const document_type& document)
{
    return avatar_data{};
}
