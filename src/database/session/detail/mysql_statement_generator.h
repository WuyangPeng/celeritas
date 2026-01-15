#pragma once

#include "database/basic/database_entity_change.h"
#include "database/basic/database_field.h"

#include <memory>
#include <string>

namespace celeritas
{
    class mysql_statement_generator final
    {
    public:
        using class_type = mysql_statement_generator;
        using database_field_container = std::vector<database_field>;
        using const_database_entity_change_shared_ptr = std::shared_ptr<const database_entity_change>;

        [[nodiscard]] static std::string generate_insert_statement(const const_database_entity_change_shared_ptr& database);

        [[nodiscard]] static std::string generate_update_statement(const const_database_entity_change_shared_ptr& database);

        [[nodiscard]] static std::string generate_delete_statement(const const_database_entity_change_shared_ptr& database);

        [[nodiscard]] static std::string generate_select_statement(const database_field_container& field_name_container, const const_database_entity_change_shared_ptr& database);

    private:
        template <typename C, typename F>
        static void join_container(std::stringstream& ss, const C& container, std::string_view separator, std::string_view tail_separator, F&& func);
    };
}