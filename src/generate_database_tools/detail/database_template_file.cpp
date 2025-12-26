#include "database_template_file.h"
#include "common/celeritas_error.h"
#include "generate_database_tools/generate_database_tools_fwd.h"

#include <boost/filesystem/path.hpp>

#include <fstream>

using namespace std::literals;

celeritas::database_template_file::database_template_file(std::string template_directory)
    : template_directory_{ std::move(template_directory) },
      entity_h_content_{},
      entity_cpp_content_{},
      database_describe_content_{},
      database_get_declaration_content_{},
      database_set_declaration_content_{},
      database_modify_declaration_content_{},
      database_array_modify_declaration_content_{},
      database_get_define_content_{},
      database_set_define_content_{},
      database_modify_define_content_{},
      database_array_modify_define_content_{},
      field_content_{},
      field_assignment_content_{},
      field_init_content_{},
      database_field_content_{},
      mongo_database_field_content_{},
      database_add_modify_content_{}
{
    load_template();
}

std::string celeritas::database_template_file::get_entity_h_content() const
{
    return entity_h_content_;
}

std::string celeritas::database_template_file::get_entity_cpp_content() const
{
    return entity_cpp_content_;
}

std::string celeritas::database_template_file::get_database_describe_content() const
{
    return database_describe_content_;
}

std::string celeritas::database_template_file::get_database_get_declaration_content() const
{
    return database_get_declaration_content_;
}

std::string celeritas::database_template_file::get_database_set_declaration_content() const
{
    return database_set_declaration_content_;
}

std::string celeritas::database_template_file::get_database_modify_declaration_content() const
{
    return database_modify_declaration_content_;
}

std::string celeritas::database_template_file::get_database_array_modify_declaration_content() const
{
    return database_array_modify_declaration_content_;
}

std::string celeritas::database_template_file::get_database_get_define_content() const
{
    return database_get_define_content_;
}

std::string celeritas::database_template_file::get_database_set_define_content() const
{
    return database_set_define_content_;
}

std::string celeritas::database_template_file::get_database_modify_define_content() const
{
    return database_modify_define_content_;
}

std::string celeritas::database_template_file::get_database_array_modify_define_content() const
{
    return database_array_modify_define_content_;
}

std::string celeritas::database_template_file::get_field_content() const
{
    return field_content_;
}

std::string celeritas::database_template_file::get_field_assignment_content() const
{
    return field_assignment_content_;
}

std::string celeritas::database_template_file::get_field_init_content() const
{
    return field_init_content_;
}

std::string celeritas::database_template_file::get_database_field_content() const
{
    return database_field_content_;
}

std::string celeritas::database_template_file::get_mongo_database_field_content() const
{
    return mongo_database_field_content_;
}

std::string celeritas::database_template_file::get_database_add_modify_content() const
{
    return database_add_modify_content_;
}

void celeritas::database_template_file::load_template()
{
    load_template(entity_h_template_name, entity_h_content_);
    load_template(entity_cpp_template_name, entity_cpp_content_);
    load_template(database_describe_template_name, database_describe_content_);
    load_template(database_get_declaration_template_name, database_get_declaration_content_);
    load_template(database_set_declaration_template_name, database_set_declaration_content_);
    load_template(database_modify_declaration_template_name, database_modify_declaration_content_);
    load_template(database_array_modify_declaration_template_name, database_array_modify_declaration_content_);
    load_template(database_get_define_template_name, database_get_define_content_);
    load_template(database_set_define_template_name, database_set_define_content_);
    load_template(database_modify_define_template_name, database_modify_define_content_);
    load_template(database_array_modify_define_template_name, database_array_modify_define_content_);
    load_template(field_template_name, field_content_);
    load_template(field_assignment_template_name, field_assignment_content_);
    load_template(field_init_template_name, field_init_content_);
    load_template(database_field_template_name, database_field_content_);
    load_template(mongo_database_field_template_name, mongo_database_field_content_);
    load_template(database_add_modify_template_name, database_add_modify_content_);
}

void celeritas::database_template_file::load_template(std::string_view template_name, std::string& content) const
{
    const boost::filesystem::path path{ template_directory_ };
    const auto file_name = path / template_name;

    std::ifstream is{ file_name.string(), std::ios::binary };
    if (!is)
    {
        throw celeritas_error("failed to load "s + template_name.data());
    }

    content.assign(std::istreambuf_iterator{ is }, std::istreambuf_iterator<char>());
}