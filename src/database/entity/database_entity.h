#pragma once

#include "config/config_fwd.h"
#include "database/basic/basis_database_container.h"
#include "database/basic/database_entity_change.h"

namespace celeritas
{
    class database_entity
    {
    public:
        using class_type = database_entity;
        using database_field_container = std::vector<database_field>;
        using const_database_entity_change_shared_ptr = std::shared_ptr<const database_entity_change>;
        using const_basis_database_container_shared_ptr = std::shared_ptr<const basis_database_container>;

        database_entity(database_type database_type, std::string_view database_name, const const_basis_database_container_shared_ptr& key);

        explicit database_entity(const database_entity_change& entity);

        database_entity(database_type database_type, const database_entity_change& entity);

        virtual ~database_entity() noexcept = default;

        database_entity(const database_entity& rhs) = default;

        database_entity& operator=(const database_entity& rhs) = default;

        database_entity(database_entity&& rhs) noexcept = default;

        database_entity& operator=(database_entity&& rhs) noexcept = default;

        [[nodiscard]] const_database_entity_change_shared_ptr get_modify() const;

        [[nodiscard]] const_database_entity_change_shared_ptr get_delete() const;

        void clear_modify();

        [[nodiscard]] bool is_modify() const;

        [[nodiscard]] bool is_must_save() const;

    protected:
        using database_entity_change_shared_ptr = std::shared_ptr<database_entity_change>;

        void add_modify(const basis_database& basis_database);

        template <typename T>
        void add_modify(std::string_view field_name, T value)
        {
            add_modify(basis_database{ field_name, value });
        }

    private:
        void detach();

        database_entity_change_shared_ptr modify_;
    };
}