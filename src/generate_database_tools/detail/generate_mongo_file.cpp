#include "generate_mongo_file.h"
#include "common/core/celeritas_error.h"

#include <filesystem>
#include <fstream>
#include <sstream>

celeritas::generate_mongo_file::generate_mongo_file(std::string database_file)
    : database_file_{ std::move(database_file) }
{
}

std::string celeritas::generate_mongo_file::get_mongo_statement() const
{
    const auto json_value = get_json_value();

    std::string result{};

    for (const auto& table_value : json_value.as_array())
    {
        result += get_mongo_statement(table_value);
    }

    return result;
}

celeritas::generate_mongo_file::json_value_type celeritas::generate_mongo_file::get_json_value() const
{
    std::ifstream file_stream{ database_file_ };
    if (!file_stream.is_open())
    {
        throw celeritas_error("Failed to open file: " + database_file_);
    }

    std::stringstream ss{};
    ss << file_stream.rdbuf();
    const auto json_content = ss.str();

    return boost::json::parse(json_content);
}

std::string celeritas::generate_mongo_file::get_mongo_statement(const json_value_type& table_value)
{
    std::stringstream mongo_output{};

    const auto& table_object = table_value.as_object();
    const auto database_name = boost::json::value_to<std::string>(table_object.at("database_name"));

    index_type unique_indexes{};
    index_type indexes{};
    index_type composite_unique_indexes{};
    index_type composite_indexes{};

    for (const auto& entities = table_object.at("entity").as_array();
         const auto& entity_value : entities)
    {
        const auto& entity_object = entity_value.as_object();
        const auto entity_name = boost::json::value_to<std::string>(entity_object.at("entity_name"));

        if (const auto index_type = entity_object.count("index_type") ? boost::json::value_to<std::string>(entity_object.at("index_type")) : "";
            index_type == "unique_index")
        {
            unique_indexes.emplace_back(entity_name);
        }
        else if (index_type == "index")
        {
            indexes.emplace_back(entity_name);
        }
        else if (index_type == "composite_unique_index")
        {
            composite_unique_indexes.emplace_back(entity_name);
        }
        else if (index_type == "composite_index")
        {
            composite_indexes.emplace_back(entity_name);
        }
        else if (index_type == "composite_unique_key_and_index")
        {
            indexes.emplace_back(entity_name);
            composite_unique_indexes.emplace_back(entity_name);
        }
    }

    for (const auto& unique_index : unique_indexes)
    {
        mongo_output << "db." << database_name << ".createIndex({ \"" << unique_index << "\": 1 }, { unique: true });\n";
    }

    for (const auto& index : indexes)
    {
        mongo_output << "db." << database_name << ".createIndex({ \"" << index << "\": 1 });\n";
    }

    if (!composite_unique_indexes.empty())
    {
        mongo_output << "db." << database_name << ".createIndex({ ";
        for (size_t i = 0; i < composite_unique_indexes.size(); ++i)
        {
            mongo_output << "\"" << composite_unique_indexes[i] << "\": 1";
            if (i < composite_unique_indexes.size() - 1)
            {
                mongo_output << ", ";
            }
        }
        mongo_output << " }, { unique: true });\n";
    }

    if (!composite_indexes.empty())
    {
        mongo_output << "db." << database_name << ".createIndex({ ";
        for (size_t i = 0; i < composite_indexes.size(); ++i)
        {
            mongo_output << "\"" << composite_indexes[i] << "\": 1";
            if (i < composite_indexes.size() - 1)
            {
                mongo_output << ", ";
            }
        }
        mongo_output << " });\n";
    }

    return mongo_output.str();
}
