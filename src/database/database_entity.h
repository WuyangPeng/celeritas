#pragma once

#include "basis_database_container.h"
#include "basis_database_manager.h"
#include "config/config_fwd.h"

namespace celeritas
{
    class database_entity
    {
    public:
        using class_type = database_entity;
        using database_field_container = std::vector<database_field>;

        database_entity(database_type database_type, std::string_view database_name, const basis_database_container& key);

        explicit database_entity(const basis_database_manager& entity);

        virtual ~database_entity() noexcept = default;

        database_entity(const database_entity& rhs) = default;

        database_entity& operator=(const database_entity& rhs) = default;

        database_entity(database_entity&& rhs) noexcept = default;

        database_entity& operator=(database_entity&& rhs) noexcept = default;

        [[nodiscard]] basis_database_manager get_modify() const;

        [[nodiscard]] basis_database_manager get_delete() const;

        void clear_modify();

        [[nodiscard]] bool is_modify() const;

    protected:
        void add_modify(const basis_database& basis_database);

        template <typename T>
        void add_modify(std::string_view field_name, T value);

    private:
        basis_database_container entity_;
        basis_database_manager modify_;
    };
}