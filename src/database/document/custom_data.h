#pragma once

#include "avatar_data.h"
#include "consumable_data.h"
#include "equipment_data.h"
#include "frame_data.h"
#include "hero_data.h"
#include "title_data.h"
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

        explicit custom_data(std::string_view type);

        [[nodiscard]] document_type to_document_type() const;

        [[nodiscard]] static custom_data from_document(const document_type& document);

        static constexpr std::string_view type_description = "type";
        static constexpr std::string_view data_description = "data";
        static constexpr std::string_view custom_description = "custom";
        static constexpr std::string_view consumable_description = "consumable";
        static constexpr std::string_view equipment_description = "equipment";
        static constexpr std::string_view avatar_description = "avatar";
        static constexpr std::string_view frame_description = "frame";
        static constexpr std::string_view title_description = "title";
        static constexpr std::string_view hero_description = "hero";

        [[nodiscard]] config::item_type get_kind() const noexcept;

        [[nodiscard]] const consumable_data* get_consumable() const;

        [[nodiscard]] const equipment_data* get_equipment() const;

        [[nodiscard]] const avatar_data* get_avatar() const;

        [[nodiscard]] const frame_data* get_frame() const;

        [[nodiscard]] const title_data* get_title() const;

        [[nodiscard]] const hero_data* get_hero() const;

    private:
        using variant_type = std::variant<std::monostate, consumable_data, equipment_data, avatar_data, frame_data, title_data, hero_data>;

        [[nodiscard]] static std::string get_type(const document_type& document);

        [[nodiscard]] static custom_data from_equipment_description(const document_type& document);

        [[nodiscard]] static custom_data from_consumable_description(const document_type& document);

        [[nodiscard]] static custom_data from_avatar_description(const document_type& document);

        [[nodiscard]] static custom_data from_frame_description(const document_type& document);

        [[nodiscard]] static custom_data from_title_description(const document_type& document);

        [[nodiscard]] static custom_data from_hero_description(const document_type& document);
 [[nodiscard]] static variant_type get_variant_type(std::string_view type);

        variant_type detail_;
    };
}