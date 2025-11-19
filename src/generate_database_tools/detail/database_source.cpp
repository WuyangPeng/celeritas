#include "database_attribute.h"
#include "database_source.h"
#include "database_template_file.h"

#include <boost/algorithm/string.hpp>

void celeritas::database_source::generate(const database_attribute& attribute, const database_template_file& database_template_file)
{
    auto index = 0;
    for (const auto& element : attribute)
    {
        if (element.is_count_type())
        {
            database_modify_define_ += create_database_modify_define_content(element, database_template_file);
        }

        if (!element.is_key_type())
        {
            field_init_ += create_field_init_content(index, attribute, element, database_template_file);
        }

        database_get_define_ += create_database_get_define_content(element, database_template_file);
        database_set_define_ += create_database_set_define_content(element, database_template_file);
        field_assignment_ += create_field_assignment_content(index, attribute, element, database_template_file);
        database_field_ += create_database_field_content(index, attribute, element, database_template_file);

        ++index;
    }
}

const std::string& celeritas::database_source::get_database_get_define() const noexcept
{
    return database_get_define_;
}

const std::string& celeritas::database_source::get_database_set_define() const noexcept
{
    return database_set_define_;
}

const std::string& celeritas::database_source::get_database_modify_define() const noexcept
{
    return database_modify_define_;
}

const std::string& celeritas::database_source::get_field_assignment() const noexcept
{
    return field_assignment_;
}

const std::string& celeritas::database_source::get_field_init() const noexcept
{
    return field_init_;
}

const std::string& celeritas::database_source::get_database_field() const noexcept
{
    return database_field_;
}

std::string celeritas::database_source::create_database_get_define_content(const entity_attribute& entity_attribute, const database_template_file& database_template_file)
{
    auto database_get_define_content = database_template_file.get_database_get_define_content();

    boost::replace_all(database_get_define_content, "${entity_type}", entity_attribute.get_data_type());
    boost::replace_all(database_get_define_content, "${entity}", entity_attribute.get_entity_name());

    if (entity_attribute.is_bool_type())
    {
        boost::replace_all(database_get_define_content, "${is_bool}", "is");
    }
    else
    {
        boost::replace_all(database_get_define_content, "${is_bool}", "get");
    }

    if (entity_attribute.is_noexcept_type())
    {
        boost::replace_all(database_get_define_content, "${entity_is_noexcept}", " noexcept");
    }
    else
    {
        boost::replace_all(database_get_define_content, "${entity_is_noexcept}", "");
    }

    return database_get_define_content;
}

std::string celeritas::database_source::create_database_set_define_content(const entity_attribute& entity_attribute, const database_template_file& database_template_file)
{
    auto database_set_define_content = database_template_file.get_database_set_define_content();

    boost::replace_all(database_set_define_content, "${entity_type}", entity_attribute.get_data_type());
    boost::replace_all(database_set_define_content, "${entity}", entity_attribute.get_entity_name());

    if (entity_attribute.is_bool_type())
    {
        boost::replace_all(database_set_define_content, "${is_bool}", "is");
    }
    else
    {
        boost::replace_all(database_set_define_content, "${is_bool}", "get");
    }

    return database_set_define_content;
}

std::string celeritas::database_source::create_database_modify_define_content(const entity_attribute& entity_attribute, const database_template_file& database_template_file)
{
    auto database_modify_define_content = database_template_file.get_database_modify_define_content();

    boost::replace_all(database_modify_define_content, "${entity_type}", entity_attribute.get_data_type());
    boost::replace_all(database_modify_define_content, "${entity}", entity_attribute.get_entity_name());

    return database_modify_define_content;
}

std::string celeritas::database_source::create_field_assignment_content(const int index, const database_attribute& attribute, const entity_attribute& entity_attribute, const database_template_file& database_template_file)
{
    auto field_assignment_content = database_template_file.get_field_assignment_content();

    boost::replace_all(field_assignment_content, "${entity_type}", entity_attribute.get_data_type());
    boost::replace_all(field_assignment_content, "${entity}", entity_attribute.get_entity_name());

    if (index + 1 == attribute.size())
    {
        boost::replace_all(field_assignment_content, "${field_is_end}", "");
    }
    else
    {
        boost::replace_all(field_assignment_content, "${field_is_end}", ",");
    }

    return field_assignment_content;
}

std::string celeritas::database_source::create_field_init_content(const int index, const database_attribute& attribute, const entity_attribute& entity_attribute, const database_template_file& database_template_file)
{
    auto field_init_content = database_template_file.get_field_init_content();

    boost::replace_all(field_init_content, "${entity_type}", entity_attribute.get_data_type());
    boost::replace_all(field_init_content, "${entity}", entity_attribute.get_entity_name());

    if (index + 1 == attribute.size())
    {
        boost::replace_all(field_init_content, "${field_is_end}", "");
    }
    else
    {
        boost::replace_all(field_init_content, "${field_is_end}", ",");
    }

    return field_init_content;
}

std::string celeritas::database_source::create_database_field_content(int index, const database_attribute& attribute, const entity_attribute& entity_attribute, const database_template_file& database_template_file)
{
    auto database_field_content = database_template_file.get_database_field_content();

    boost::replace_all(database_field_content, "${entity}", entity_attribute.get_entity_name());
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

    return database_field_content;
}

