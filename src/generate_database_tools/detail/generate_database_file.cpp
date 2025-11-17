#include "database_attribute_container.h"
#include "generate_database_file.h"
#include "common/celeritas_error.h"

#include <boost/json.hpp>

#include <fstream>
#include <sstream>

celeritas::generate_database_file::generate_database_file(std::string relative_path, std::string database_file, std::string output_directory, const database_template_file& database_template_file)
    : relative_path_{ std::move(relative_path) }, database_file_{ std::move(database_file) }, output_directory_{ std::move(output_directory) }, database_template_file_{ database_template_file }
{
}

void celeritas::generate_database_file::generate_file()
{
    std::ifstream file_stream{ database_file_ };
    if (!file_stream.is_open())
    {
        throw celeritas_error("Failed to open file: " + database_file_);
    }

    std::stringstream ss{};
    ss << file_stream.rdbuf();
    const auto json_content = ss.str();
    const auto json_value = boost::json::parse(json_content);

    const auto container = boost::json::value_to<database_attribute_container>(json_value);
}