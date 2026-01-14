#include "redis_row_data_converter.h"
#include "common/core/celeritas_error.h"

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

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

celeritas::redis_row_data_converter::const_basis_database_container_shared_ptr celeritas::redis_row_data_converter::get_key(const std::string& key, const const_database_entity_change_shared_ptr& database)
{
    const auto extracted_key_values = get_key_value(key);

    const auto& key_type = *database->get_key();
    if (extracted_key_values.size() != key_type.get_size())
    {
        throw celeritas_error{ "key size is error." };
    }

    basis_database_container::object_container objects{};
    auto index = 0;
    for (const auto& element : key_type)
    {
        objects.emplace_back(element.get_field_name(), extracted_key_values.at(index));
        ++index;
    }

    return std::make_shared<basis_database_container>(objects);
}

celeritas::redis_row_data_converter::array_type celeritas::redis_row_data_converter::get_key_value(const std::string& key)
{
    array_type parts{};
    boost::split(parts, key, boost::is_any_of(":"), boost::token_compress_off);

    if (parts.size() < 2)
    {
        throw celeritas_error{ "redis key size is error." };
    }

    const auto& key_values = parts.back();

    array_type result{};
    boost::split(result, key_values, boost::is_any_of("_"), boost::token_compress_off);

    return result;
}