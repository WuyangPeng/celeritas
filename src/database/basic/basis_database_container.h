#pragma once

#include "basis_database.h"

#include <vector>

namespace celeritas
{
    class basis_database_container
    {
    public:
        using class_type = basis_database_container;
        using value_variant = basis_database::value_variant;
        using object_container = std::vector<basis_database>;
        using object_container_const_iter = object_container::const_iterator;

        basis_database_container() noexcept = default;

        explicit basis_database_container(const basis_database& basisDatabase);

        explicit basis_database_container(object_container container);

        void modify(const basis_database& basis_database);

        void clear();

        [[nodiscard]] const value_variant& get_variant_value(std::string_view field_name) const;

        [[nodiscard]] object_container_const_iter begin() const noexcept;

        [[nodiscard]] object_container_const_iter end() const noexcept;

        [[nodiscard]] int get_size() const;

    private:
        object_container container_;
    };
}