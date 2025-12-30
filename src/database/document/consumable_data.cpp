#include "consumable_data.h"
#include "common/core/celeritas_error.h"
#include "database/basis_database.tpp"

#include <bsoncxx/types.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>

celeritas::consumable_data::consumable_data()
    : expire_time_{ 0 }
{
}

celeritas::consumable_data::consumable_data(const int64_t expire_time)
    : expire_time_{ expire_time }
{
}

int64_t celeritas::consumable_data::get_expire_time() const
{
    return expire_time_;
}

void celeritas::consumable_data::set_expire_time(const int64_t expire_time)
{
    expire_time_ = expire_time;
}

celeritas::consumable_data::document_type celeritas::consumable_data::to_document_type() const
{
    document_type document{};

    document.emplace_back(expire_time_description, expire_time_);

    return document;
}

celeritas::consumable_data celeritas::consumable_data::from_document(const document_type& document)
{
    consumable_data consumable_data{};

    for (const auto& element : document)
    {
        if (element.get_field_name() == expire_time_description)
        {
            consumable_data.set_expire_time(element.get_value<database_data_type::int64_type>());
        }
    }

    return consumable_data;
}
