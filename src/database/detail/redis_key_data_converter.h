#pragma once

#include "database/basic/basis_database.h"
#include "database/basic/database_entity_change.h"

#include <memory>
#include <string>
#include <vector>

namespace celeritas
{
    class redis_key_data_converter
    {
    public:
        using class_type = redis_key_data_converter;
        using database_entity_change_const_shared_ptr = std::shared_ptr<const database_entity_change>;
        using basis_database_container_const_shared_ptr = std::shared_ptr<const basis_database_container>;

        [[nodiscard]] static std::string generate_key(const database_entity_change_const_shared_ptr& database);

        [[nodiscard]] static basis_database get_basis_database(const database_field& field_name, const std::string& value);

        [[nodiscard]] static basis_database get_basis_database(const database_field& field_name);

        [[nodiscard]] static basis_database_container_const_shared_ptr get_key(const std::string& key, const database_entity_change_const_shared_ptr& database);

    private:
        using array_type = std::vector<std::string>;

        [[nodiscard]] static array_type get_key_value(const std::string& key);

        template <typename ArrayType>
        [[nodiscard]] static basis_database to_numeric_array_basis(const database_field& field_name, const std::string& value);
    };
}