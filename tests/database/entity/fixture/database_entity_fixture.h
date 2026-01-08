#pragma once
#include "config/basic/database_type.h"
#include "database/basic/basis_database_container.h"

namespace celeritas
{
    class database_entity_fixture
    {
    public:
        using class_type = database_entity_fixture;
        using basis_database_container_shared_ptr = std::shared_ptr<basis_database_container>;

        database_entity_fixture();

        [[nodiscard]] basis_database_container_shared_ptr get_key() const;

        [[nodiscard]] database_type get_db_type() const;

        [[nodiscard]] std::string get_db_name() const;

    private:
        basis_database_container_shared_ptr key_;
        database_type db_type_;
        std::string db_name_;
    };
}