#include "generate_database_file.h"

celeritas::generate_database_file::generate_database_file(std::string relative_path, std::string database_file, std::string output_directory, const database_template_file& database_template_file)
    : relative_path_{ std::move(relative_path) }, database_file_{ std::move(database_file) }, output_directory_{ std::move(output_directory) }, database_template_file_{ database_template_file }
{
}

void celeritas::generate_database_file::generate_file()
{
}