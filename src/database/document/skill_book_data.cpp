#include "skill_book_data.h"
#include "database/basic/basis_database.tpp"

celeritas::skill_book_data::skill_book_data()
{
}

celeritas::skill_book_data::document_type celeritas::skill_book_data::to_document_type() const
{
    return document_type{};
}

celeritas::skill_book_data celeritas::skill_book_data::from_document(const document_type& document)
{
    return skill_book_data{};
}
