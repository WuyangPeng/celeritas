#include "database_attribute_container.h"
#include "database_header.h"
#include "database_source.h"
#include "database_template_file.h"
#include "generate_database_file.h"
#include "common/core/celeritas_error.h"
#include "common/logging/logger.h"

#include <boost/json.hpp>
#include <boost/algorithm/string.hpp>

#include <filesystem>
#include <fstream>
#include <sstream>

celeritas::generate_database_file::generate_database_file(std::string relative_path, std::string database_file, std::string output_directory, const database_template_file& database_template_file, const bool mongo)
    : relative_path_{ std::move(relative_path) }, database_file_{ std::move(database_file) }, output_directory_{ std::move(output_directory) }, database_template_file_{ database_template_file }, mongo_{ mongo }
{
}

void celeritas::generate_database_file::execute()
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

    for (const auto container = boost::json::value_to<database_attribute_container>(json_value);
         const auto& attribute : container)
    {
        generate_file(attribute);
    }
}

void celeritas::generate_database_file::generate_file(const database_attribute& attribute)
{
    generate_entity_h_file(attribute);
    generate_entity_cpp_file(attribute);
}

void celeritas::generate_database_file::generate_entity_h_file(const database_attribute& attribute)
{
    const auto class_name = attribute.get_class_name();

    LOG_CHANNEL(celeritas::default_channel, trace) << "generate from file : " << database_file_ << ",class header file:" << class_name;

    const auto entity_h_content = generate_header_content(attribute);

    std::filesystem::path path{ output_directory_ };
    path /= relative_path_;

    std::filesystem::path database_file_path{ database_file_ };
    auto file_name = database_file_path.filename();
    file_name = file_name.replace_extension("");
    path /= file_name;

    std::filesystem::create_directory(path);
    path /= class_name + ".h";

    if (const auto file_name = path.string();
        !is_content_same(file_name, entity_h_content))
    {
        save_database(file_name, entity_h_content);
    }
}

void celeritas::generate_database_file::generate_entity_cpp_file(const database_attribute& attribute)
{
    const auto class_name = attribute.get_class_name();

    LOG_CHANNEL(celeritas::default_channel, trace) << "generate from file : " << database_file_ << ",class source file:" << class_name;

    const auto entity_cpp_content = generate_source_content(attribute);

    std::filesystem::path path{ output_directory_ };
    path /= relative_path_;

    const std::filesystem::path database_file_path{ database_file_ };
    auto file_name = database_file_path.filename();
    file_name = file_name.replace_extension("");
    path /= file_name;

    std::filesystem::create_directory(path);
    path /= class_name + ".cpp";

    if (const auto file_name_path = path.string();
        !is_content_same(file_name_path, entity_cpp_content))
    {
        save_database(file_name_path, entity_cpp_content);
    }
}

std::string celeritas::generate_database_file::generate_header_content(const database_attribute& attribute) const
{
    auto entity_h_content = database_template_file_.get_entity_h_content();

    boost::replace_all(entity_h_content, "${class_name}", attribute.get_class_name());
    boost::replace_all(entity_h_content, "${database_name}", attribute.get_database_name());
    boost::replace_all(entity_h_content, "${key_type}", attribute.get_key_type());
    boost::replace_all(entity_h_content, "${key_name}", attribute.get_key_name());

    database_header header{};
    header.generate(attribute, database_template_file_, mongo_);

    boost::replace_all(entity_h_content, "${database_get_declaration}", header.get_database_get_declaration());
    boost::replace_all(entity_h_content, "${database_set_declaration}", header.get_database_set_declaration());
    boost::replace_all(entity_h_content, "${database_modify_declaration}", header.get_database_modify_declaration());
    boost::replace_all(entity_h_content, "${database_array_modify_declaration}", header.get_database_array_modify_declaration());
    boost::replace_all(entity_h_content, "${declaration}", header.get_database_describe());
    boost::replace_all(entity_h_content, "${field}", header.get_field());

    return entity_h_content;
}

std::string celeritas::generate_database_file::generate_source_content(const database_attribute& attribute) const
{
    auto entity_cpp_content = database_template_file_.get_entity_cpp_content();

    boost::replace_all(entity_cpp_content, "${key_type}", attribute.get_key_type());
    boost::replace_all(entity_cpp_content, "${key_name}", attribute.get_key_name());

    database_source source{};
    source.generate(attribute, database_template_file_);

    boost::replace_all(entity_cpp_content, "${database_get_define}", source.get_database_get_define());
    boost::replace_all(entity_cpp_content, "${database_set_define}", source.get_database_set_define());
    boost::replace_all(entity_cpp_content, "${database_modify_define}", source.get_database_modify_define());
    boost::replace_all(entity_cpp_content, "${database_array_modify_define}", source.get_database_array_modify_define());
    boost::replace_all(entity_cpp_content, "${field_assignment}", source.get_field_assignment());
    boost::replace_all(entity_cpp_content, "${field_init}", source.get_field_init());
    boost::replace_all(entity_cpp_content, "${class_name}", attribute.get_class_name());
    boost::replace_all(entity_cpp_content, "${database_field}", source.get_database_field());
    boost::replace_all(entity_cpp_content, "${add_modify}", source.get_database_add_modify());

    return entity_cpp_content;
}
