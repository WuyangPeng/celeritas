#include "mysql_statement_generator.tpp"

using namespace std::literals;

std::string celeritas::mysql_statement_generator::generate_insert_statement(const const_database_entity_change_shared_ptr& database)
{
    std::string result{};

    result += "INSERT INTO `";
    result += database->get_database_name();
    result += "`(";

    const auto& container = *database->get_database();
    result += join_container(container, " , ", "", [](const auto& value) {
        return "`"s + value.get_field_name().data() + "`";
    });

    result += ") VALUES(";

    result += join_container(container, " , ", "", [](const auto& value) {
        return value.get_quotation_mark_string();
    });

    result += ");";

    return result;
}

std::string celeritas::mysql_statement_generator::generate_update_statement(const const_database_entity_change_shared_ptr& database)
{
    std::string result{};

    result += "UPDATE `";
    result += database->get_database_name();
    result += "` SET ";

    const auto container = *database->get_database();

    result += join_container(container, " , ", "", [](const auto& value) {
        return "`"s + value.get_field_name().data() + "` = " + value.get_sql_field_string();
    });

    result += "WHERE ";

    const auto& key = *database->get_key();

    result += join_container(key, " AND ", "", [](const auto& value) {
        return "`"s + value.get_field_name().data() + "` = " + value.get_sql_field_string();
    });

    result += " LIMIT 1;";

    return result;
}

std::string celeritas::mysql_statement_generator::generate_delete_statement(const const_database_entity_change_shared_ptr& database)
{
    std::string result{};

    result += "DELETE FROM `";
    result += database->get_database_name();

    const auto& key = *database->get_key();
    if (key.get_size() != 0)
    {
        result += "` WHERE ";
    }

    result += join_container(key, " AND ", "", [](const auto& value) {
        return "`"s + value.get_field_name().data() + "` = " + value.get_sql_field_string();
    });

    result += " LIMIT 1;";

    return result;
}

std::string celeritas::mysql_statement_generator::generate_select_statement(const database_field_container& field_name_container, const const_database_entity_change_shared_ptr& database)
{
    std::string result{};

    result += "SELECT ";

    result += join_container(field_name_container, " , ", " ", [](const auto& value) {
        return "`"s + value.get_field_name().data() + "`";
    });

    result += "FROM `";
    result += database->get_database_name();

    const auto& key = *database->get_key();

    if (key.get_size() != 0)
    {
        result += "` WHERE ";
    }
    else
    {
        result += "` ";
    }

    result += join_container(key, " AND ", "", [](const auto& value) {
        return "`"s + value.get_field_name().data() + "` = " + value.get_quotation_mark_string();
    });

    return result;
}

