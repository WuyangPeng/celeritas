#include "logs_data.h"
#include "common/core/celeritas_error.h"
#include "database/basic/basis_database.tpp"

#include <bsoncxx/types.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>

celeritas::logs_data::logs_data()
    : expire_time_{ 0 }
{
}

celeritas::logs_data::logs_data(const int64_t expire_time)
    : expire_time_{ expire_time }
{
}

int64_t celeritas::logs_data::get_expire_time() const
{
    return expire_time_;
}

void celeritas::logs_data::set_expire_time(const int64_t expire_time)
{
    expire_time_ = expire_time;
}

celeritas::logs_data::document_type celeritas::logs_data::to_document_type() const
{
    document_type document{};

    document.emplace_back(expire_time_description, expire_time_);

    return document;
}

celeritas::logs_data celeritas::logs_data::from_document(const document_type& document)
{
    logs_data logs_data{};

    for (const auto& element : document)
    {
        if (element.get_field_name() == expire_time_description)
        {
            logs_data.set_expire_time(element.get_value<database_data_type::int64_type>());
        }
    }

    return logs_data;
}
