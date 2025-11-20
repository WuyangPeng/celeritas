#include "save_mysql_file.h"

#include <filesystem>

celeritas::save_mysql_file::save_mysql_file(std::string output_directory, std::string content)
    : output_directory_{ std::move(output_directory) }, content_{ std::move(content) }
{
}

void celeritas::save_mysql_file::execute()
{
    std::filesystem::path path{ output_directory_ };
    path /= "schema.sql";

    if (const auto file_name = path.string();
        !is_content_same(file_name, content_))
    {
        save_database(file_name, content_);
    }
}