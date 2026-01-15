#include "redis_row_data_converter.h"
#include "common/core/celeritas_error.h"

#include <boost/algorithm/string/classification.hpp>

std::string celeritas::redis_row_data_converter::generate_key(const const_database_entity_change_shared_ptr& database)
{
    std::string result{};
    result += database->get_database_name();
    result += ":";

    const auto& keys = *database->get_key();
    for (auto iter = keys.begin(); iter != keys.end(); ++iter)
    {
        result += iter->get_string();

        if (std::next(iter) != keys.end())
        {
            result += "_";
        }
    }

    return result;
}
