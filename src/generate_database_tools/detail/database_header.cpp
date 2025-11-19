#include "database_attribute.h"
#include "database_header.h"
#include "database_template_file.h"

#include <boost/algorithm/string.hpp>

void celeritas::database_header::generate(const database_attribute& attribute, const database_template_file& database_template_file)
{
    for (const auto& element : attribute)
    {
        if (element.is_count_type())
        {
            database_modify_declaration_ += create_database_modify_declaration_content(element, database_template_file);
        }

        database_get_declaration_ += create_database_get_declaration_content(element, database_template_file);
        database_set_declaration_ += create_database_set_declaration_content(element, database_template_file);
        database_describe_ += create_database_describe_content(element, database_template_file);
        field_ += create_field_content(element, database_template_file);
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

std::string celeritas::database_header::create_database_get_declaration_content(const entity_attribute& entity_attribute, const database_template_file& database_template_file)
{
    auto database_get_declaration_content = database_template_file.get_database_get_declaration_content();

    boost::replace_all(database_get_declaration_content, "${entity_type}", entity_attribute.get_data_type());
    boost::replace_all(database_get_declaration_content, "${entity}", entity_attribute.get_entity_name());

    if (entity_attribute.is_bool_type())
    {
        boost::replace_all(database_get_declaration_content, "${is_bool}", "is");
    }
    else
    {
        boost::replace_all(database_get_declaration_content, "${is_bool}", "get");
    }

    if (entity_attribute.is_noexcept_type())
    {
        boost::replace_all(database_get_declaration_content, "${entity_is_noexcept}", " noexcept");
    }
    else
    {
        boost::replace_all(database_get_declaration_content, "${entity_is_noexcept}", "");
    }

    return database_get_declaration_content;
}

std::string celeritas::database_header::create_database_set_declaration_content(const entity_attribute& entity_attribute, const database_template_file& database_template_file)
{
    auto database_set_declaration_content = database_template_file.get_database_set_declaration_content();

    boost::replace_all(database_set_declaration_content, "${entity_type}", entity_attribute.get_data_type());
    boost::replace_all(database_set_declaration_content, "${entity}", entity_attribute.get_entity_name());

    return database_set_declaration_content;
}

std::string celeritas::database_header::create_database_modify_declaration_content(const entity_attribute& entity_attribute, const database_template_file& database_template_file)
{
    auto database_modify_declaration_content = database_template_file.get_database_modify_declaration_content();

    boost::replace_all(database_modify_declaration_content, "${entity_type}", entity_attribute.get_data_type());
    boost::replace_all(database_modify_declaration_content, "${entity}", entity_attribute.get_entity_name());

    return database_modify_declaration_content;
}

std::string celeritas::database_header::create_database_describe_content(const entity_attribute& entity_attribute, const database_template_file& database_template_file)
{
    auto database_describe_content = database_template_file.get_database_describe_content();

    if (entity_attribute.is_key_type())
    {
        boost::replace_all(database_describe_content, "${entity_is_key}", "_id");
    }
    else
    {
        boost::replace_all(database_describe_content, "${entity_is_key}", entity_attribute.get_entity_name());
    }

    boost::replace_all(database_describe_content, "${entity}", entity_attribute.get_entity_name());

    return database_describe_content;
}

std::string celeritas::database_header::create_field_content(const entity_attribute& entity_attribute, const database_template_file& database_template_file)
{
    auto field_content = database_template_file.get_field_content();

    boost::replace_all(field_content, "${entity}", entity_attribute.get_entity_name());
    boost::replace_all(field_content, "${entity_type}", entity_attribute.get_data_type());

    if (const auto index_type = entity_attribute.get_index_type();
        index_type.has_value())
    {
        boost::replace_all(field_content, "${entity_is_index}", ", database_index_type::" + *index_type);
    }
    else
    {
        boost::replace_all(field_content, "${entity_is_index}", "");
    }
    return field_content;
}

