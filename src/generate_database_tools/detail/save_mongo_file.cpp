#include "save_mongo_file.h"

#include <filesystem>
#include <fstream>

celeritas::save_mongo_file::save_mongo_file(std::string file_name, std::string output_directory, std::string content)
    : file_name_{ std::move(file_name) },
      output_directory_{ std::move(output_directory) },
      content_{ std::move(content) }
{
}

void celeritas::save_mongo_file::execute()
{
    std::filesystem::path path{ output_directory_ };
    path /= file_name_;
    path.replace_extension(".js");

    std::ofstream file(path);
    file << content_;
}

