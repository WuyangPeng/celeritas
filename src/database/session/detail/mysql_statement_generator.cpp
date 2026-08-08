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

void celeritas::mysql_statement_generator::append_where_clause(std::stringstream& result, const const_database_entity_change_shared_ptr& database)
{
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
}

void celeritas::mysql_statement_generator::append_where_clause(std::stringstream& result,
                                                               const const_database_entity_change_shared_ptr& database,
                                                               const database_select_options& options)
{
    const auto& key = *database->get_key();
    const auto& like_conditions = options.get_like_conditions();
    const bool has_eq = key.get_size() != 0;
    const bool has_like = !like_conditions.empty();

    if (has_eq || has_like)
    {
        result << "` WHERE ";
    }
    else
    {
        result << "` ";
    }

    // 等值条件
    join_container(result, key, " AND ", "", [&result](const auto& value) {
        result << "`" << value.get_field_name() << "` = " << value.get_sql_value_string();
    });

    // LIKE 条件（每组字段 OR，组间 AND）
    for (const auto& like : like_conditions)
    {
        if (has_eq || (&like != &like_conditions.front()))
        {
            result << " AND ";
        }

        result << "(";
        bool first = true;
        for (const auto& field : like.field_names)
        {
            if (!first) result << " OR ";
            first = false;
            result << "`" << field << "` LIKE '" << "%" << like.pattern << "%'";
        }
        result << ")";
    }
}

void celeritas::mysql_statement_generator::append_select_options(std::stringstream& result, const database_select_options& options)
{
    const auto& order_by_container = options.get_order_by();
    if (!order_by_container.empty())
    {
        result << " ORDER BY ";
        join_container(result, order_by_container, " , ", "", [&result](const auto& order_by) {
            result << "`" << order_by.field_name << "` " << (order_by.order == sort_order::desc ? "DESC" : "ASC");
        });
    }

    if (const auto limit = options.get_limit())
    {
        result << " LIMIT " << *limit;
    }

    if (const auto offset = options.get_offset())
    {
        result << " OFFSET " << *offset;
    }
}

std::string celeritas::mysql_statement_generator::generate_select_statement(const database_field_container& field_name_container, const const_database_entity_change_shared_ptr& database)
{
    std::stringstream result{};

    result << "SELECT ";

    join_container(result, field_name_container, " , ", " ", [&result](const auto& value) {
        result << "`" << value.get_field_name() << "`";
    });

    result << "FROM `" << database->get_database_name();

    append_where_clause(result, database);

    return result.str();
}

std::string celeritas::mysql_statement_generator::generate_select_statement(const database_field_container& field_name_container,
                                                                           const const_database_entity_change_shared_ptr& database,
                                                                           const database_select_options& options)
{
    options.validate(field_name_container);

    std::stringstream result{};

    result << "SELECT ";
    join_container(result, field_name_container, " , ", " ", [&result](const auto& value) {
        result << "`" << value.get_field_name() << "`";
    });
    result << "FROM `" << database->get_database_name();
    append_where_clause(result, database, options);
    append_select_options(result, options);

    return result.str();
}

std::string celeritas::mysql_statement_generator::generate_count_statement(const const_database_entity_change_shared_ptr& database)
{
    std::stringstream result{};

    result << "SELECT COUNT(*) FROM `" << database->get_database_name();
    append_where_clause(result, database);

    return result.str();
}

std::string celeritas::mysql_statement_generator::generate_count_statement(const const_database_entity_change_shared_ptr& database,
                                                                           const database_select_options& options)
{
    std::stringstream result{};

    result << "SELECT COUNT(*) FROM `" << database->get_database_name();
    append_where_clause(result, database, options);

    return result.str();
}
