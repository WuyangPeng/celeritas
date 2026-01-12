#include "properties_data.h"
#include "common/core/celeritas_error.h"
#include "database/basic/basis_database.tpp"

#include <bsoncxx/types.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>

celeritas::properties_data::properties_data()
    : expire_time_{ 0 }
{
}

celeritas::properties_data::properties_data(const int64_t expire_time)
    : expire_time_{ expire_time }
{
}

int64_t celeritas::properties_data::get_expire_time() const
{
    return expire_time_;
}

void celeritas::properties_data::set_expire_time(const int64_t expire_time)
{
    expire_time_ = expire_time;
}

celeritas::properties_data::document_type celeritas::properties_data::to_document_type() const
{
    document_type document{};

    document.emplace_back(expire_time_description, expire_time_);

    return document;
}

celeritas::properties_data celeritas::properties_data::from_document(const document_type& document)
{
    properties_data properties_data{};

    for (const auto& element : document)
    {
        if (element.get_field_name() == expire_time_description)
        {
            properties_data.set_expire_time(element.get_value<database_data_type::int64_type>());
        }
    }

    return properties_data;
}
