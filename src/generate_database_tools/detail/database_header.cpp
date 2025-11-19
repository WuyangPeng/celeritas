#include "database_attribute.h"
#include "database_header.h"
#include "database_template_file.h"

#include <boost/algorithm/string.hpp>

void celeritas::database_header::generate(const database_attribute& attribute, const database_template_file& database_template_file)
{
    for (const auto& element : attribute)
    {
        auto database_get_declaration_content = database_template_file.get_database_get_declaration_content();
        auto database_set_declaration_content = database_template_file.get_database_set_declaration_content();
        auto database_describe_content = database_template_file.get_database_describe_content();
        auto field_content = database_template_file.get_field_content();

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
            auto database_modify_declaration_content = database_template_file.get_database_modify_declaration_content();

            boost::replace_all(database_modify_declaration_content, "${entity_type}", element.get_data_type());
            boost::replace_all(database_modify_declaration_content, "${entity}", element.get_entity_name());

            database_modify_declaration_ += database_modify_declaration_content;
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

        database_get_declaration_ += database_get_declaration_content;
        database_set_declaration_ += database_set_declaration_content;
        database_describe_ += database_describe_content;
        field_ += field_content;
    }
}

const std::string& celeritas::database_header::get_database_get_declaration() const noexcept
{
    return database_get_declaration_;
}

const std::string& celeritas::database_header::get_database_set_declaration() const noexcept
{
    return database_set_declaration_;
}

const std::string& celeritas::database_header::get_database_modify_declaration() const noexcept
{
    return database_modify_declaration_;
}

const std::string& celeritas::database_header::get_database_describe() const noexcept
{
    return database_describe_;
}

const std::string& celeritas::database_header::get_field() const noexcept
{
    return field_;
}

