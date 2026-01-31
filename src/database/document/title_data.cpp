#include "title_data.h"
#include "database/basic/basis_database.tpp"

celeritas::title_data::document_type celeritas::title_data::to_document_type() const
{
    return document_type{};
}

celeritas::title_data celeritas::title_data::from_document(const document_type& document)
{
    return title_data{};
}
