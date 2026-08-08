#pragma once

#include "avatar_data.h"
#include "building_data.h"
#include "consumable_data.h"
#include "equipment_data.h"
#include "exp_data.h"
#include "frame_data.h"
#include "hero_data.h"
#include "resource_data.h"
#include "soldier_data.h"
#include "machine_data.h"
#include "skill_book_data.h"
#include "blueprint_data.h"
#include "gift_box_data.h"
#include "treasure_data.h"
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
        static constexpr std::string_view exp_description = "exp";
        static constexpr std::string_view building_description = "building";
        static constexpr std::string_view resource_description = "resource";
        static constexpr std::string_view soldier_description = "soldier";
        static constexpr std::string_view machine_description = "machine";
        static constexpr std::string_view skill_book_description = "skill_book";
        static constexpr std::string_view blueprint_description = "blueprint";
        static constexpr std::string_view gift_box_description = "gift_box";
        static constexpr std::string_view treasure_description = "treasure";

        [[nodiscard]] config::item_type get_kind() const noexcept;

        [[nodiscard]] const consumable_data* get_consumable() const;

        [[nodiscard]] const equipment_data* get_equipment() const;

        [[nodiscard]] const avatar_data* get_avatar() const;

        [[nodiscard]] const frame_data* get_frame() const;

        [[nodiscard]] const title_data* get_title() const;

        [[nodiscard]] const hero_data* get_hero() const;

        [[nodiscard]] const building_data* get_building() const;

        [[nodiscard]] const resource_data* get_resource() const;

        [[nodiscard]] const soldier_data* get_soldier() const;

        [[nodiscard]] const machine_data* get_machine() const;

        [[nodiscard]] const skill_book_data* get_skill_book() const;

        [[nodiscard]] const blueprint_data* get_blueprint() const;

        [[nodiscard]] const gift_box_data* get_gift_box() const;

        [[nodiscard]] const treasure_data* get_treasure() const;

    private:
        using variant_type = std::variant<
            std::monostate,
            consumable_data,
            equipment_data,
            avatar_data,
            frame_data,
            title_data,
            hero_data,
            exp_data,
            building_data,
            resource_data,
            soldier_data,
            machine_data,
            skill_book_data,
            blueprint_data,
            gift_box_data,
            treasure_data
        >;

        [[nodiscard]] static std::string get_type(const document_type& document);

        [[nodiscard]] static custom_data from_equipment_description(const document_type& document);

        [[nodiscard]] static custom_data from_consumable_description(const document_type& document);

        [[nodiscard]] static custom_data from_avatar_description(const document_type& document);

        [[nodiscard]] static custom_data from_frame_description(const document_type& document);

        [[nodiscard]] static custom_data from_title_description(const document_type& document);

        [[nodiscard]] static custom_data from_hero_description(const document_type& document);

        [[nodiscard]] static custom_data from_building_description(const document_type& document);

        [[nodiscard]] static custom_data from_resource_description(const document_type& document);

        [[nodiscard]] static custom_data from_soldier_description(const document_type& document);

        [[nodiscard]] static custom_data from_machine_description(const document_type& document);

        [[nodiscard]] static custom_data from_skill_book_description(const document_type& document);

        [[nodiscard]] static custom_data from_blueprint_description(const document_type& document);

        [[nodiscard]] static custom_data from_gift_box_description(const document_type& document);

        [[nodiscard]] static custom_data from_treasure_description(const document_type& document);

        [[nodiscard]] static variant_type get_variant_type(std::string_view type);

        variant_type detail_;
    };
}