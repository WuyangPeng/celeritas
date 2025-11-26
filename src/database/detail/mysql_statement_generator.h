#pragma once

#include "database/database_entity_change.h"
#include "database/database_field.h"

#include <memory>
#include <string>

namespace celeritas
{
    class mysql_statement_generator final
    {
    public:
        using class_type = mysql_statement_generator;
        using database_field_container = std::vector<database_field>;
        using basis_database_manager_const_shared_ptr = std::shared_ptr<const database_entity_change>;

        [[nodiscard]] static std::string generate_insert_statement(const basis_database_manager_const_shared_ptr& database);

        [[nodiscard]] static std::string generate_update_statement(const basis_database_manager_const_shared_ptr& database);

        [[nodiscard]] static std::string generate_delete_statement(const basis_database_manager_const_shared_ptr& database);

        [[nodiscard]] static std::string generate_select_statement(const database_field_container& field_name_container, const database_entity_change& database);
    };
}