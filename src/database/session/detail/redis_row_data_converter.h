#pragma once

#include "database/basic/basis_database.h"
#include "database/basic/database_entity_change.h"

#include <memory>
#include <string>
#include <vector>

namespace celeritas
{
    class redis_row_data_converter
    {
    public:
        using class_type = redis_row_data_converter;
        using const_database_entity_change_shared_ptr = std::shared_ptr<const database_entity_change>;
        using const_basis_database_container_shared_ptr = std::shared_ptr<const basis_database_container>;

        [[nodiscard]] static std::string generate_key(const const_database_entity_change_shared_ptr& database);

        [[nodiscard]] static const_basis_database_container_shared_ptr get_key(const std::string& key, const const_database_entity_change_shared_ptr& database);

    private:
        using array_type = std::vector<std::string>;

        [[nodiscard]] static array_type get_key_value(const std::string& key);
    };
}