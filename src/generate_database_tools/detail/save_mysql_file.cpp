#include "save_mysql_file.h"

#include <boost/algorithm/string.hpp>

#include <filesystem>

celeritas::save_mysql_file::save_mysql_file(std::string file_name, std::string output_directory, std::string content)
    : file_name_{ std::move(file_name) }, output_directory_{ std::move(output_directory) }, content_{ std::move(content) }
{
    boost::replace_all(file_name_, ".json", ".sql");
}

void celeritas::save_mysql_file::execute()
{
    std::filesystem::path path{ output_directory_ };
    path /= file_name_;

    if (const auto file_name = path.string();
        !is_content_same(file_name, content_))
    {
        save_database(file_name, content_);
    }
}