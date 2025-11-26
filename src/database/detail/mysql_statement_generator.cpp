#include "mysql_statement_generator.h"

using namespace std::literals;

std::string celeritas::mysql_statement_generator::generate_insert_statement(const database_entity_change_const_shared_ptr& database)
{
    std::string result{};

    result += "INSERT INTO `";
    result += database->get_database_name();
    result += "`(";

    const auto container = database->get_database();
    auto index = 1;
    for (const auto& value : *container)
    {
        result += "`";
        result += value.get_field_name();
        result += "`";

        if (index != container->get_size())
        {
            result += " , ";
        }

        ++index;
    }

    result += ") VALUES(";

    index = 1;
    for (const auto& value : *container)
    {
        result += value.get_quotation_mark_string();

        if (index != container->get_size())
        {
            result += " , ";
        }

        ++index;
    }

    result += ");";

    return result;
}

std::string celeritas::mysql_statement_generator::generate_update_statement(const database_entity_change_const_shared_ptr& database)
{
    std::string result{};

    result += "UPDATE `";
    result += database->get_database_name();
    result += "` SET ";

    const auto container = database->get_database();
    auto index = 1;
    for (const auto& value : *container)
    {
        result += "`";
        result += value.get_field_name();
        result += "` = ";
        result += value.get_sql_field_string();

        if (index != container->get_size())
        {
            result += " , ";
        }

        ++index;
    }

    result += "WHERE ";

    const auto key = database->get_key();

    index = 1;
    for (const auto& value : *key)
    {
        result += "`";
        result += value.get_field_name();
        result += "` = ";
        result += value.get_sql_field_string();

        if (index != key->get_size())
        {
            result += " AND ";
        }

        ++index;
    }

    result += " LIMIT 1;";

    return result;
}

std::string celeritas::mysql_statement_generator::generate_delete_statement(const database_entity_change_const_shared_ptr& database)
{
    std::string result{};

    result += "DELETE FROM `";
    result += database->get_database_name();

    const auto key = database->get_key();
    if (key->get_size() != 0)
    {
        result += "` WHERE ";
    }

    auto index = 1;
    for (const auto& value : *key)
    {
        result += "`";
        result += value.get_field_name();
        result += "` = ";
        result += value.get_sql_field_string();

        if (index != key->get_size())
        {
            result += " AND ";
        }

        ++index;
    }

    result += " LIMIT 1;";

    return result;
}

std::string celeritas::mysql_statement_generator::generate_select_statement(const database_field_container& field_name_container, const database_entity_change_const_shared_ptr& database)
{
    std::string result{};

    result += "SELECT ";

    auto index = 1u;
    for (const auto& value : field_name_container)
    {
        result += "`";
        result += value.get_field_name();
        result += "`";

        if (index != field_name_container.size())
        {
            result += " , ";
        }
        else
        {
            result += " ";
        }

        ++index;
    }

    result += "FROM `";
    result += database->get_database_name();

    const auto key = database->get_key();

    if (key->get_size() != 0)
    {
        result += "` WHERE ";
    }
    else
    {
        result += "` ";
    }

    auto keyIndex = 1;
    for (const auto& value : *key)
    {
        result += "`";
        result += value.get_field_name();
        result += "` = ";

        result += value.get_quotation_mark_string();

        if (keyIndex != key->get_size())
        {
            result += " AND ";
        }

        ++keyIndex;
    }

    return result;
}