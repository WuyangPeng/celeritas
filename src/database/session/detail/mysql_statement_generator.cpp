#include "mysql_statement_generator.tpp"

std::string celeritas::mysql_statement_generator::generate_insert_statement(const const_database_entity_change_shared_ptr& database)
{
    std::stringstream result{};

    result << "INSERT INTO `" << database->get_database_name() << "`(";

    const auto& container = *database->get_database();
    join_container(result, container, " , ", "", [&result](const auto& value) {
        result << "`" << value.get_field_name() << "`";
    });

    result << ") VALUES(";

    join_container(result, container, " , ", "", [&result](const auto& value) {
        result << value.get_sql_value_string();
    });

    result << ");";

    return result.str();
}

std::string celeritas::mysql_statement_generator::generate_update_statement(const const_database_entity_change_shared_ptr& database)
{
    std::stringstream result{};

    result << "UPDATE `" << database->get_database_name() << "` SET ";

    const auto& container = *database->get_database();

    join_container(result, container, " , ", "", [&result](const auto& value) {
        result << "`" << value.get_field_name() << "` = " << value.get_sql_value_string();
    });

    result << " WHERE ";

    const auto& key = *database->get_key();

    join_container(result, key, " AND ", "", [&result](const auto& value) {
        result << "`" << value.get_field_name() << "` = " << value.get_sql_value_string();
    });

    result << " LIMIT 1;";

    return result.str();
}

std::string celeritas::mysql_statement_generator::generate_delete_statement(const const_database_entity_change_shared_ptr& database)
{
    std::stringstream result{};

    result << "DELETE FROM `" << database->get_database_name();

    const auto& key = *database->get_key();
    if (key.get_size() != 0)
    {
        result << "` WHERE ";
    }

    join_container(result, key, " AND ", "", [&result](const auto& value) {
        result << "`" << value.get_field_name() << "` = " << value.get_sql_value_string();
    });

    result << " LIMIT 1;";

    return result.str();
}

std::string celeritas::mysql_statement_generator::generate_select_statement(const database_field_container& field_name_container, const const_database_entity_change_shared_ptr& database)
{
    std::stringstream result{};

    result << "SELECT ";

    join_container(result, field_name_container, " , ", " ", [&result](const auto& value) {
        result << "`" << value.get_field_name() << "`";
    });

    result << "FROM `" << database->get_database_name();

    const auto& key = *database->get_key();

    if (key.get_size() != 0)
    {
        result << "` WHERE ";
    }
    else
    {
        result << "` ";
    }

    join_container(result, key, " AND ", "", [&result](const auto& value) {
        result << "`" << value.get_field_name() << "` = " << value.get_sql_value_string();
    });

    return result.str();
}
