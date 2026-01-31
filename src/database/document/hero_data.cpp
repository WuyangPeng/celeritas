#include "hero_data.h"
#include "database/basic/basis_database.tpp"

celeritas::hero_data::document_type celeritas::hero_data::to_document_type() const
{
    return document_type{};
}

celeritas::hero_data celeritas::hero_data::from_document(const document_type& document)
{
    return hero_data{};
}
