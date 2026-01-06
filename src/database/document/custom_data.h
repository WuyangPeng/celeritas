#pragma once

#include "consumable_data.h"
#include "equipment_data.h"
#include "database/basic/database_data_type_traits.h"

#include <bsoncxx/builder/basic/document.hpp>
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

    private:
        using variant_type = std::variant<std::monostate, equipment_data, consumable_data>;

        variant_type detail_;
    };
}