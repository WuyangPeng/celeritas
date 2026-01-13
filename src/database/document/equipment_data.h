#pragma once

#include "database/basic/database_data_type_traits.h"

namespace celeritas
{
    class equipment_data
    {
    public:
        using class_type = equipment_data;
        using document_type = traits::document_type;

        equipment_data();

        explicit equipment_data(int strength, int durability);

        [[nodiscard]] int get_strength() const;

        void set_strength(int strength);

        [[nodiscard]] int get_durability() const;

        void set_durability(int durability);

        [[nodiscard]] document_type to_document_type() const;

        [[nodiscard]] static equipment_data from_document(const document_type& document);

        static constexpr std::string_view strength_description = "strength";
        static constexpr std::string_view durability_description = "durability";

    private:
        int strength_;
        int durability_;
    };
}
