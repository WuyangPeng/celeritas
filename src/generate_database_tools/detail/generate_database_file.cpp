#include "database_attribute_container.h"
#include "generate_database_file.h"
#include "common/celeritas_error.h"
#include "common/logger.h"

#include <boost/json.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/path.hpp>

#include <filesystem>
#include <fstream>
#include <sstream>

celeritas::generate_database_file::generate_database_file(std::string relative_path, std::string database_file, std::string output_directory, const database_template_file& database_template_file)
    : relative_path_{ std::move(relative_path) }, database_file_{ std::move(database_file) }, output_directory_{ std::move(output_directory) }, database_template_file_{ database_template_file }
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

    auto entity_h_content = database_template_file_.get_entity_h_content();

    boost::replace_all(entity_h_content, "${class_name}", class_name);
    boost::replace_all(entity_h_content, "${database_name}", attribute.get_database_name());
    boost::replace_all(entity_h_content, "${key_type}", attribute.get_key_type());
    boost::replace_all(entity_h_content, "${key_name}", attribute.get_key_name());

    std::string database_get_declaration{};
    std::string database_set_declaration{};
    std::string database_modify_declaration{};
    std::string database_describe{};
    std::string field{};

    for (const auto& element : attribute)
    {
        auto database_get_declaration_content = database_template_file_.get_database_get_declaration_content();
        auto database_set_declaration_content = database_template_file_.get_database_set_declaration_content();
        auto database_describe_content = database_template_file_.get_database_describe_content();
        auto field_content = database_template_file_.get_field_content();

        boost::replace_all(database_get_declaration_content, "${entity_type}", element.get_data_type());
        boost::replace_all(database_get_declaration_content, "${entity}", element.get_entity_name());

        if (element.get_data_type() == "bool_type")
        {
            boost::replace_all(database_get_declaration_content, "${is_bool}", "is");
        }
        else
        {
            boost::replace_all(database_get_declaration_content, "${is_bool}", "get");
        }

        if (element.get_data_type() == "int32_type" ||
            element.get_data_type() == "int32_count_type" ||
            element.get_data_type() == "int64_type" ||
            element.get_data_type() == "int64_count_type" ||
            element.get_data_type() == "double_type" ||
            element.get_data_type() == "bool_type")
        {
            boost::replace_all(database_get_declaration_content, "${entity_is_noexcept}", " noexcept");
        }
        else
        {
            boost::replace_all(database_get_declaration_content, "${entity_is_noexcept}", "");
        }

        boost::replace_all(database_set_declaration_content, "${entity_type}", element.get_data_type());
        boost::replace_all(database_set_declaration_content, "${entity}", element.get_entity_name());

        if (element.get_data_type().find("_count") != std::string::npos)
        {
            auto database_modify_declaration_content = database_template_file_.get_database_modify_declaration_content();

            boost::replace_all(database_modify_declaration_content, "${entity_type}", element.get_data_type());
            boost::replace_all(database_modify_declaration_content, "${entity}", element.get_entity_name());

            database_modify_declaration += database_modify_declaration_content;
        }
        const auto index_type = element.get_index_type();
        if (index_type.has_value() && index_type->find("key") != std::string::npos)
        {
            boost::replace_all(database_describe_content, "${entity_is_key}", "_id");
        }
        else
        {
            boost::replace_all(database_describe_content, "${entity_is_key}", element.get_entity_name());
        }
        boost::replace_all(database_describe_content, "${entity}", element.get_entity_name());

        boost::replace_all(field_content, "${entity}", element.get_entity_name());
        boost::replace_all(field_content, "${entity_type}", element.get_data_type());
        if (index_type.has_value())
        {
            boost::replace_all(field_content, "${entity_is_index}", ", database_index_type::" + *index_type);
        }
        else
        {
            boost::replace_all(field_content, "${entity_is_index}", "");
        }

        database_get_declaration += database_get_declaration_content;
        database_set_declaration += database_set_declaration_content;
        database_describe += database_describe_content;
        field += field_content;
    }

    boost::replace_all(entity_h_content, "${database_get_declaration}", database_get_declaration);
    boost::replace_all(entity_h_content, "${database_set_declaration}", database_set_declaration);
    boost::replace_all(entity_h_content, "${database_modify_declaration}", database_modify_declaration);
    boost::replace_all(entity_h_content, "${declaration}", database_describe);
    boost::replace_all(entity_h_content, "${field}", field);

    std::filesystem::path path{ output_directory_ };
    path /= relative_path_;
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

    auto entity_cpp_content = database_template_file_.get_entity_cpp_content();

    boost::replace_all(entity_cpp_content, "${key_type}", attribute.get_key_type());
    boost::replace_all(entity_cpp_content, "${key_name}", attribute.get_key_name());

    std::string database_get_define{};
    std::string database_set_define{};
    std::string database_modify_define{};
    std::string field_assignment{};
    std::string field_init{};
    std::string database_field{};

    auto index = 0;
    for (const auto& element : attribute)
    {
        auto database_get_define_content = database_template_file_.get_database_get_define_content();
        auto database_set_define_content = database_template_file_.get_database_set_define_content();
        auto field_assignment_content = database_template_file_.get_field_assignment_content();
        auto field_init_content = database_template_file_.get_field_init_content();
        auto database_field_content = database_template_file_.get_database_field_content();

        boost::replace_all(database_get_define_content, "${entity_type}", element.get_data_type());
        boost::replace_all(database_get_define_content, "${entity}", element.get_entity_name());

        if (element.get_data_type() == "bool_type")
        {
            boost::replace_all(database_get_define_content, "${is_bool}", "is");
        }
        else
        {
            boost::replace_all(database_get_define_content, "${is_bool}", "get");
        }

        if (element.get_data_type() == "int32_type" ||
            element.get_data_type() == "int32_count_type" ||
            element.get_data_type() == "int64_type" ||
            element.get_data_type() == "int64_count_type" ||
            element.get_data_type() == "double_type" ||
            element.get_data_type() == "bool_type")
        {
            boost::replace_all(database_get_define_content, "${entity_is_noexcept}", " noexcept");
        }
        else
        {
            boost::replace_all(database_get_define_content, "${entity_is_noexcept}", "");
        }

        boost::replace_all(database_set_define_content, "${entity_type}", element.get_data_type());
        boost::replace_all(database_set_define_content, "${entity}", element.get_entity_name());

        if (element.get_data_type() == "bool_type")
        {
            boost::replace_all(database_set_define_content, "${is_bool}", "is");
        }
        else
        {
            boost::replace_all(database_set_define_content, "${is_bool}", "get");
        }

        if (element.get_data_type().find("_count") != std::string::npos)
        {
            auto database_modify_define_content = database_template_file_.get_database_modify_define_content();

            boost::replace_all(database_modify_define_content, "${entity_type}", element.get_data_type());
            boost::replace_all(database_modify_define_content, "${entity}", element.get_entity_name());

            database_modify_define += database_modify_define_content;
        }

        boost::replace_all(field_assignment_content, "${entity_type}", element.get_data_type());
        boost::replace_all(field_assignment_content, "${entity}", element.get_entity_name());

        if (index + 1 == attribute.size())
        {
            boost::replace_all(field_assignment_content, "${field_is_end}", "");
        }
        else
        {
            boost::replace_all(field_assignment_content, "${field_is_end}", ",");
        }

        boost::replace_all(field_init_content, "${entity_type}", element.get_data_type());
        boost::replace_all(field_init_content, "${entity}", element.get_entity_name());

        if (index + 1 == attribute.size())
        {
            boost::replace_all(field_init_content, "${field_is_end}", "");
        }
        else
        {
            boost::replace_all(field_init_content, "${field_is_end}", ",");
        }

        if (const auto index_type = element.get_index_type();
            !index_type.has_value() ||
            index_type->find("key") == std::string::npos)
        {
            field_init += field_init_content;
        }

        boost::replace_all(database_field_content, "${entity}", element.get_entity_name());
        if (index + 1 == attribute.size())
        {
            boost::replace_all(database_field_content, "${field_is_end}", "");
        }
        else
        {
            boost::replace_all(database_field_content, "${field_is_end}", ",\n");
        }

        if (index == 0)
        {
            boost::replace_all(database_field_content, "${entity_indent}", "");
        }
        else
        {
            boost::replace_all(database_field_content, "${entity_indent}", "                                                                ");
        }

        database_get_define += database_get_define_content;
        database_set_define += database_set_define_content;
        field_assignment += field_assignment_content;
        database_field += database_field_content;

        ++index;
    }

    boost::replace_all(entity_cpp_content, "${database_get_define}", database_get_define);
    boost::replace_all(entity_cpp_content, "${database_set_define}", database_set_define);
    boost::replace_all(entity_cpp_content, "${database_modify_define}", database_modify_define);
    boost::replace_all(entity_cpp_content, "${field_assignment}", field_assignment);
    boost::replace_all(entity_cpp_content, "${field_init}", field_init);
    boost::replace_all(entity_cpp_content, "${class_name}", class_name);
    boost::replace_all(entity_cpp_content, "${database_field}", database_field);

    std::filesystem::path path{ output_directory_ };
    path /= relative_path_;
    std::filesystem::create_directory(path);
    path /= class_name + ".cpp";

    if (const auto file_name = path.string();
        !is_content_same(file_name, entity_cpp_content))
    {
        save_database(file_name, entity_cpp_content);
    }
}