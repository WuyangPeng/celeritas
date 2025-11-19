#include "database_attribute.h"
#include "database_source.h"
#include "database_template_file.h"

#include <boost/algorithm/string.hpp>

void celeritas::database_source::generate(const database_attribute& attribute, const database_template_file& database_template_file)
{
    auto index = 0;
    for (const auto& element : attribute)
    {
        auto database_get_define_content = database_template_file.get_database_get_define_content();
        auto database_set_define_content = database_template_file.get_database_set_define_content();
        auto field_assignment_content = database_template_file.get_field_assignment_content();
        auto field_init_content = database_template_file.get_field_init_content();
        auto database_field_content = database_template_file.get_database_field_content();

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
            auto database_modify_define_content = database_template_file.get_database_modify_define_content();

            boost::replace_all(database_modify_define_content, "${entity_type}", element.get_data_type());
            boost::replace_all(database_modify_define_content, "${entity}", element.get_entity_name());

            database_modify_define_ += database_modify_define_content;
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
            field_init_ += field_init_content;
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

        database_get_define_ += database_get_define_content;
        database_set_define_ += database_set_define_content;
        field_assignment_ += field_assignment_content;
        database_field_ += database_field_content;

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

