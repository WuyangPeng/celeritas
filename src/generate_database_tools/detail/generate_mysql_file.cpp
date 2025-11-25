#include "generate_mysql_file.h"
#include "common/celeritas_error.h"
#include "common/logger.h"

#include <boost/algorithm/string.hpp>

#include <filesystem>
#include <fstream>
#include <sstream>

celeritas::generate_mysql_file::generate_mysql_file(std::string database_file)
    : database_file_{ std::move(database_file) }
{
}

std::string celeritas::generate_mysql_file::get_mysql_statement() const
{
    const auto json_value = get_json_value();

    std::string result{};

    for (const auto& table_value : json_value.as_array())
    {
        result += get_mysql_statement(table_value);
    }

    return result;
}

std::string celeritas::generate_mysql_file::get_mysql_data_type(const std::string& data_type)
{
    if (data_type == "int32" || data_type == "int32_count")
    {
        return "INT";
    }

    if (data_type == "int64" || data_type == "int64_count")
    {
        return "BIGINT";
    }

    if (data_type == "string")
    {
        return "VARCHAR(255)";
    }

    if (data_type == "float")
    {
        return "FLOAT";
    }

    if (data_type == "double")
    {
        return "DOUBLE";
    }

    if (data_type == "bool")
    {
        return "TINYINT(1)";
    }

    if (data_type == "binary")
    {
        return "BLOB";
    }

    LOG_CHANNEL(celeritas::default_channel, error) << "Unknown data type: " << data_type;

    return "TEXT";
}

std::string celeritas::generate_mysql_file::get_mysql_default_type(const std::string& data_type)
{
    if (data_type == "int32" || data_type == "int32_count")
    {
        return "0";
    }

    if (data_type == "int64" || data_type == "int64_count")
    {
        return "0";
    }

    if (data_type == "string")
    {
        return "\"\"";
    }

    if (data_type == "double")
    {
        return "0.0";
    }

    if (data_type == "bool")
    {
        return "0";
    }

    LOG_CHANNEL(celeritas::default_channel, error) << "Unknown data type: " << data_type;

    return "\"\"";
}

celeritas::generate_mysql_file::json_value_type celeritas::generate_mysql_file::get_json_value() const
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

std::string celeritas::generate_mysql_file::get_mysql_statement(const json_value_type& table_value)
{
    std::stringstream sql_output{};

    const auto& table_object = table_value.as_object();
    const auto database_name = boost::json::value_to<std::string>(table_object.at("database_name"));

    sql_output << "CREATE TABLE `" << database_name << "` (\n";

    std::string primary_key_sql{};
    index_type unique_indexes{};
    index_type indexes{};
    index_type composite_unique_indexes{};
    index_type composite_indexes{};

    for (const auto& entities = table_object.at("entity").as_array();
         const auto& entity_value : entities)
    {
        const auto& entity_object = entity_value.as_object();
        const auto entity_name = boost::json::value_to<std::string>(entity_object.at("entity_name"));
        const auto data_type = boost::json::value_to<std::string>(entity_object.at("data_type"));
        const auto comment = entity_object.count("comment") ? boost::json::value_to<std::string>(entity_object.at("comment")) : "";
        const auto index_type = entity_object.count("index_type") ? boost::json::value_to<std::string>(entity_object.at("index_type")) : "";

        sql_output << "  `" << entity_name << "` " << get_mysql_data_type(data_type);

        sql_output << " NOT NULL";

        if (index_type == "key")
        {
            primary_key_sql = "  PRIMARY KEY (`" + entity_name + "`)";
        }
        else if (index_type == "unique_index")
        {
            unique_indexes.emplace_back("  UNIQUE KEY `" + entity_name + "_unique` (`" + entity_name + "`)");
        }
        else if (index_type == "index")
        {
            indexes.emplace_back("  KEY `" + entity_name + "_index` (`" + entity_name + "`)");
        }
        else if (index_type == "composite_unique_index")
        {
            composite_unique_indexes.emplace_back(entity_name);
        }
        else if (index_type == "composite_index")
        {
            composite_indexes.emplace_back(entity_name);
        }

        if (data_type != "binary" && index_type != "key" && index_type != "unique_index" && index_type != "composite_unique_index")
        {
            sql_output << " DEFAULT " << get_mysql_default_type(data_type);
        }

        if (!comment.empty())
        {
            sql_output << " COMMENT '" << comment << "'";
        }
        sql_output << ",\n";
    }

    if (!primary_key_sql.empty())
    {
        sql_output << primary_key_sql << ",\n";
    }

    for (const auto& unique_index : unique_indexes)
    {
        sql_output << unique_index << ",\n";
    }

    for (const auto& index : indexes)
    {
        sql_output << index << ",\n";
    }

    if (!composite_unique_indexes.empty())
    {
        sql_output << "  UNIQUE KEY `";
    }

    for (const auto& index : composite_unique_indexes)
    {
        sql_output << index << '_';
    }

    if (!composite_unique_indexes.empty())
    {
        sql_output << "unique` (";
    }

    auto current_index = 0;
    for (const auto& index : composite_unique_indexes)
    {
        sql_output << '`' << index;
        ++current_index;
        if (current_index == composite_unique_indexes.size())
        {
            sql_output << "`),\n";
        }
        else
        {
            sql_output << "`,";
        }
    }

    if (!composite_indexes.empty())
    {
        sql_output << "  KEY `";
    }

    for (const auto& index : composite_indexes)
    {
        sql_output << index << '_';
    }

    if (!composite_indexes.empty())
    {
        sql_output << "index` (";
    }

    current_index = 0;
    for (const auto& index : composite_indexes)
    {
        sql_output << '`' << index;
        ++current_index;
        if (current_index == composite_indexes.size())
        {
            sql_output << "`),\n";
        }
        else
        {
            sql_output << "`,";
        }
    }

    auto temp_sql = sql_output.str();
    if (boost::algorithm::ends_with(temp_sql, ",\n"))
    {
        temp_sql.resize(temp_sql.length() - 2);
    }

    sql_output.str(temp_sql);
    sql_output.seekp(0, std::ios_base::end);

    sql_output << "\n) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;\n\n";

    return sql_output.str();
}
