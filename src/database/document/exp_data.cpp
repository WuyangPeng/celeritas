#include "exp_data.h"
#include "database/basic/basis_database.tpp"

celeritas::exp_data::exp_data()
{
}

celeritas::exp_data::document_type celeritas::exp_data::to_document_type() const
{
    return document_type{};
}

celeritas::exp_data celeritas::exp_data::from_document(const document_type& document)
{
    return exp_data{};
}
