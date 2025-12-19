#pragma once

#include <bsoncxx/builder/basic/document.hpp>

namespace celeritas
{
    class equipment_data
    {
    public:
        using class_type = equipment_data;
        using document_type = bsoncxx::builder::basic::document;
        using document_view_type = bsoncxx::document::view;

        equipment_data();

        explicit equipment_data(const document_view_type& document_view);

        explicit equipment_data(int strength, int durability);

        [[nodiscard]] int get_strength() const;

        void set_strength(int strength);

        [[nodiscard]] int get_durability() const;

        void set_durability(int durability);

        [[nodiscard]] document_type to_document_type() const;

        void set_document(const document_view_type& document_view);

        static constexpr std::string_view strength_description = "strength";
        static constexpr std::string_view durability_description = "durability";

    private:
        int strength_;
        int durability_;
    };
}
