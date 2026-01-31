#pragma once

#include "consumable_data.h"
#include "equipment_data.h"
#include "config/luban/generated/schema.h"
#include "database/basic/database_data_type_traits.h"

#include <variant>

namespace celeritas
{
    class custom_data
    {
    public:
        using class_type = custom_data;
        using document_type = traits::document_type;

        custom_data();

        [[nodiscard]] document_type to_document_type() const;

        [[nodiscard]] static custom_data from_document(const document_type& document);

        static constexpr std::string_view type_description = "type";
        static constexpr std::string_view data_description = "data";
        static constexpr std::string_view equipment_description = "equipment";
        static constexpr std::string_view consumable_description = "consumable";

        [[nodiscard]] config::item_type get_kind() const noexcept;

        [[nodiscard]] const equipment_data* get_equipment() const noexcept;

        [[nodiscard]] const consumable_data* get_consumable() const noexcept;

    private:
        using variant_type = std::variant<std::monostate, equipment_data, consumable_data>;

        [[nodiscard]] static std::string get_type(const document_type& document);

        [[nodiscard]] static custom_data from_equipment_description(const document_type& document);

        [[nodiscard]] static custom_data from_consumable_description(const document_type& document);

        variant_type detail_;
    };
}