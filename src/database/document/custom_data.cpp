#include "custom_data.h"
#include "exp_data.h"
#include "common/core/celeritas_error.h"
#include "database/basic/basis_database.tpp"

celeritas::custom_data::custom_data()
    : detail_{ std::monostate{} }
{
}

celeritas::custom_data::custom_data(std::string_view type)
    : detail_{ get_variant_type(type) }
{
}

celeritas::custom_data::document_type celeritas::custom_data::to_document_type() const
{
    document_type document{};

    std::visit(
        [&document]<typename ArgType>(ArgType&& arg) {
            using T = std::decay_t<ArgType>;
            if constexpr (std::is_same_v<T, consumable_data>)
            {
                document.emplace_back(type_description, std::string{ consumable_description });
                document.emplace_back(data_description, arg.to_document_type());
            }
            else if constexpr (std::is_same_v<T, equipment_data>)
            {
                document.emplace_back(type_description, std::string{ equipment_description });
                document.emplace_back(data_description, arg.to_document_type());
            }
            else if constexpr (std::is_same_v<T, avatar_data>)
            {
                document.emplace_back(type_description, std::string{ avatar_description });
                document.emplace_back(data_description, arg.to_document_type());
            }
            else if constexpr (std::is_same_v<T, frame_data>)
            {
                document.emplace_back(type_description, std::string{ frame_description });
                document.emplace_back(data_description, arg.to_document_type());
            }
            else if constexpr (std::is_same_v<T, title_data>)
            {
                document.emplace_back(type_description, std::string{ title_description });
                document.emplace_back(data_description, arg.to_document_type());
            }
            else if constexpr (std::is_same_v<T, hero_data>)
            {
                document.emplace_back(type_description, std::string{ hero_description });
                document.emplace_back(data_description, arg.to_document_type());
            }
            else if constexpr (std::is_same_v<T, building_data>)
            {
                document.emplace_back(type_description, std::string{ building_description });
                document.emplace_back(data_description, arg.to_document_type());
            }
            else if constexpr (std::is_same_v<T, resource_data>)
            {
                document.emplace_back(type_description, std::string{ resource_description });
                document.emplace_back(data_description, arg.to_document_type());
            }
            else if constexpr (std::is_same_v<T, soldier_data>)
            {
                document.emplace_back(type_description, std::string{ soldier_description });
                document.emplace_back(data_description, arg.to_document_type());
            }
            else if constexpr (std::is_same_v<T, machine_data>)
            {
                document.emplace_back(type_description, std::string{ machine_description });
                document.emplace_back(data_description, arg.to_document_type());
            }
            else if constexpr (std::is_same_v<T, skill_book_data>)
            {
                document.emplace_back(type_description, std::string{ skill_book_description });
                document.emplace_back(data_description, arg.to_document_type());
            }
            else if constexpr (std::is_same_v<T, blueprint_data>)
            {
                document.emplace_back(type_description, std::string{ blueprint_description });
                document.emplace_back(data_description, arg.to_document_type());
            }
            else if constexpr (std::is_same_v<T, gift_box_data>)
            {
                document.emplace_back(type_description, std::string{ gift_box_description });
                document.emplace_back(data_description, arg.to_document_type());
            }
            else if constexpr (std::is_same_v<T, treasure_data>)
            {
                document.emplace_back(type_description, std::string{ treasure_description });
                document.emplace_back(data_description, arg.to_document_type());
            }
            else if constexpr (std::is_same_v<T, std::monostate>)
            {
                // monostate结果为空文档
            }
        },
        detail_);

    return document;
}

celeritas::custom_data celeritas::custom_data::from_document(const document_type& document)
{
    const auto type = get_type(document);

    if (type.empty())
    {
        return custom_data{};
    }

    if (type == consumable_description)
    {
        return from_consumable_description(document);
    }
    if (type == equipment_description)
    {
        return from_equipment_description(document);
    }
    if (type == avatar_description)
    {
        return from_avatar_description(document);
    }
    if (type == frame_description)
    {
        return from_frame_description(document);
    }
    if (type == title_description)
    {
        return from_title_description(document);
    }
    if (type == hero_description)
    {
        return from_hero_description(document);
    }
    if (type == building_description)
    {
        return from_building_description(document);
    }
    if (type == resource_description)
    {
        return from_resource_description(document);
    }
    if (type == soldier_description)
    {
        return from_soldier_description(document);
    }
    if (type == machine_description)
    {
        return from_machine_description(document);
    }
    if (type == skill_book_description)
    {
        return from_skill_book_description(document);
    }
    if (type == blueprint_description)
    {
        return from_blueprint_description(document);
    }
    if (type == gift_box_description)
    {
        return from_gift_box_description(document);
    }
    if (type == treasure_description)
    {
        return from_treasure_description(document);
    }

    throw celeritas_error{ "custom_data::from_document() failed." };
}

celeritas::config::item_type celeritas::custom_data::get_kind() const noexcept
{
    if (std::holds_alternative<consumable_data>(detail_))
    {
        return config::item_type::consumable;
    }

    if (std::holds_alternative<equipment_data>(detail_))
    {
        return config::item_type::equipment;
    }

    if (std::holds_alternative<avatar_data>(detail_))
    {
        return config::item_type::avatar;
    }

    if (std::holds_alternative<frame_data>(detail_))
    {
        return config::item_type::frame;
    }

    if (std::holds_alternative<title_data>(detail_))
    {
        return config::item_type::title;
    }

    if (std::holds_alternative<hero_data>(detail_))
    {
        return config::item_type::hero;
    }

    if (std::holds_alternative<exp_data>(detail_))
    {
        return config::item_type::exp;
    }

    if (std::holds_alternative<building_data>(detail_))
    {
        return config::item_type::building;
    }

    if (std::holds_alternative<resource_data>(detail_))
    {
        return config::item_type::resource;
    }

    if (std::holds_alternative<soldier_data>(detail_))
    {
        return config::item_type::soldier;
    }

    if (std::holds_alternative<machine_data>(detail_))
    {
        return config::item_type::machine;
    }

    if (std::holds_alternative<skill_book_data>(detail_))
    {
        return config::item_type::skill_book;
    }

    if (std::holds_alternative<blueprint_data>(detail_))
    {
        return config::item_type::blueprint;
    }

    if (std::holds_alternative<gift_box_data>(detail_))
    {
        return config::item_type::gift_box;
    }

    if (std::holds_alternative<treasure_data>(detail_))
    {
        return config::item_type::treasure;
    }

    return config::item_type::none;
}

const celeritas::equipment_data* celeritas::custom_data::get_equipment() const
{
    if (const auto result = std::get_if<equipment_data>(&detail_))
    {
        return result;
    }

    throw celeritas_error{ "custom_data::get_equipment() failed, custom_data is not equipment." };
}

const celeritas::consumable_data* celeritas::custom_data::get_consumable() const
{
    if (const auto result = std::get_if<consumable_data>(&detail_))
    {
        return result;
    }

    throw celeritas_error{ "custom_data::get_consumable() failed, custom_data is not consumable." };
}

const celeritas::avatar_data* celeritas::custom_data::get_avatar() const
{
    if (const auto result = std::get_if<avatar_data>(&detail_))
    {
        return result;
    }

    throw celeritas_error{ "custom_data::get_avatar() failed, custom_data is not avatar." };
}

const celeritas::frame_data* celeritas::custom_data::get_frame() const
{
    if (const auto result = std::get_if<frame_data>(&detail_))
    {
        return result;
    }

    throw celeritas_error{ "custom_data::get_frame() failed, custom_data is not frame." };
}

const celeritas::title_data* celeritas::custom_data::get_title() const
{
    if (const auto result = std::get_if<title_data>(&detail_))
    {
        return result;
    }

    throw celeritas_error{ "custom_data::get_title() failed, custom_data is not title." };
}

const celeritas::hero_data* celeritas::custom_data::get_hero() const
{
    if (const auto result = std::get_if<hero_data>(&detail_))
    {
        return result;
    }

    throw celeritas_error{ "custom_data::get_hero() failed, custom_data is not hero." };
}

const celeritas::building_data* celeritas::custom_data::get_building() const
{
    if (const auto result = std::get_if<building_data>(&detail_))
    {
        return result;
    }

    throw celeritas_error{ "custom_data::get_building() failed, custom_data is not building." };
}

const celeritas::resource_data* celeritas::custom_data::get_resource() const
{
    if (const auto result = std::get_if<resource_data>(&detail_))
    {
        return result;
    }

    throw celeritas_error{ "custom_data::get_resource() failed, custom_data is not resource." };
}

const celeritas::soldier_data* celeritas::custom_data::get_soldier() const
{
    if (const auto result = std::get_if<soldier_data>(&detail_))
    {
        return result;
    }

    throw celeritas_error{ "custom_data::get_soldier() failed, custom_data is not soldier." };
}

const celeritas::machine_data* celeritas::custom_data::get_machine() const
{
    if (const auto result = std::get_if<machine_data>(&detail_))
    {
        return result;
    }

    throw celeritas_error{ "custom_data::get_machine() failed, custom_data is not machine." };
}

const celeritas::skill_book_data* celeritas::custom_data::get_skill_book() const
{
    if (const auto result = std::get_if<skill_book_data>(&detail_))
    {
        return result;
    }

    throw celeritas_error{ "custom_data::get_skill_book() failed, custom_data is not skill_book." };
}

const celeritas::blueprint_data* celeritas::custom_data::get_blueprint() const
{
    if (const auto result = std::get_if<blueprint_data>(&detail_))
    {
        return result;
    }

    throw celeritas_error{ "custom_data::get_blueprint() failed, custom_data is not blueprint." };
}

const celeritas::gift_box_data* celeritas::custom_data::get_gift_box() const
{
    if (const auto result = std::get_if<gift_box_data>(&detail_))
    {
        return result;
    }

    throw celeritas_error{ "custom_data::get_gift_box() failed, custom_data is not gift_box." };
}

const celeritas::treasure_data* celeritas::custom_data::get_treasure() const
{
    if (const auto result = std::get_if<treasure_data>(&detail_))
    {
        return result;
    }

    throw celeritas_error{ "custom_data::get_treasure() failed, custom_data is not treasure." };
}

std::string celeritas::custom_data::get_type(const document_type& document)
{
    for (const auto& element : document)
    {
        if (element.get_field_name() == type_description)
        {
            return element.get_value<database_data_type::string_type>();
        }
    }

    return "";
}

celeritas::custom_data celeritas::custom_data::from_equipment_description(const document_type& document)
{
    custom_data custom_data{};
    for (const auto& element : document)
    {
        if (element.get_field_name() == data_description)
        {
            custom_data.detail_ = equipment_data::from_document(element.get_value<database_data_type::document_type>());
            break;
        }
    }
    return custom_data;
}

celeritas::custom_data celeritas::custom_data::from_consumable_description(const document_type& document)
{
    custom_data custom_data{};
    for (const auto& element : document)
    {
        if (element.get_field_name() == data_description)
        {
            custom_data.detail_ = consumable_data::from_document(element.get_value<database_data_type::document_type>());
            break;
        }
    }
    return custom_data;
}

celeritas::custom_data celeritas::custom_data::from_avatar_description(const document_type& document)
{
    custom_data custom_data{};
    for (const auto& element : document)
    {
        if (element.get_field_name() == data_description)
        {
            custom_data.detail_ = avatar_data::from_document(element.get_value<database_data_type::document_type>());
            break;
        }
    }
    return custom_data;
}

celeritas::custom_data celeritas::custom_data::from_frame_description(const document_type& document)
{
    custom_data custom_data{};
    for (const auto& element : document)
    {
        if (element.get_field_name() == data_description)
        {
            custom_data.detail_ = frame_data::from_document(element.get_value<database_data_type::document_type>());
            break;
        }
    }
    return custom_data;
}

celeritas::custom_data celeritas::custom_data::from_title_description(const document_type& document)
{
    custom_data custom_data{};
    for (const auto& element : document)
    {
        if (element.get_field_name() == data_description)
        {
            custom_data.detail_ = title_data::from_document(element.get_value<database_data_type::document_type>());
            break;
        }
    }
    return custom_data;
}

celeritas::custom_data celeritas::custom_data::from_hero_description(const document_type& document)
{
    custom_data custom_data{};
    for (const auto& element : document)
    {
        if (element.get_field_name() == data_description)
        {
            custom_data.detail_ = hero_data::from_document(element.get_value<database_data_type::document_type>());
            break;
        }
    }
    return custom_data;
}

celeritas::custom_data celeritas::custom_data::from_building_description(const document_type& document)
{
    custom_data custom_data{};
    for (const auto& element : document)
    {
        if (element.get_field_name() == data_description)
        {
            custom_data.detail_ = building_data::from_document(element.get_value<database_data_type::document_type>());
            break;
        }
    }
    return custom_data;
}

celeritas::custom_data celeritas::custom_data::from_resource_description(const document_type& document)
{
    custom_data custom_data{};
    for (const auto& element : document)
    {
        if (element.get_field_name() == data_description)
        {
            custom_data.detail_ = resource_data::from_document(element.get_value<database_data_type::document_type>());
            break;
        }
    }
    return custom_data;
}

celeritas::custom_data celeritas::custom_data::from_soldier_description(const document_type& document)
{
    custom_data custom_data{};
    for (const auto& element : document)
    {
        if (element.get_field_name() == data_description)
        {
            custom_data.detail_ = soldier_data::from_document(element.get_value<database_data_type::document_type>());
            break;
        }
    }
    return custom_data;
}

celeritas::custom_data celeritas::custom_data::from_machine_description(const document_type& document)
{
    custom_data custom_data{};
    for (const auto& element : document)
    {
        if (element.get_field_name() == data_description)
        {
            custom_data.detail_ = machine_data::from_document(element.get_value<database_data_type::document_type>());
            break;
        }
    }
    return custom_data;
}

celeritas::custom_data celeritas::custom_data::from_skill_book_description(const document_type& document)
{
    custom_data custom_data{};
    for (const auto& element : document)
    {
        if (element.get_field_name() == data_description)
        {
            custom_data.detail_ = skill_book_data::from_document(element.get_value<database_data_type::document_type>());
            break;
        }
    }
    return custom_data;
}

celeritas::custom_data celeritas::custom_data::from_blueprint_description(const document_type& document)
{
    custom_data custom_data{};
    for (const auto& element : document)
    {
        if (element.get_field_name() == data_description)
        {
            custom_data.detail_ = blueprint_data::from_document(element.get_value<database_data_type::document_type>());
            break;
        }
    }
    return custom_data;
}

celeritas::custom_data celeritas::custom_data::from_gift_box_description(const document_type& document)
{
    custom_data custom_data{};
    for (const auto& element : document)
    {
        if (element.get_field_name() == data_description)
        {
            custom_data.detail_ = gift_box_data::from_document(element.get_value<database_data_type::document_type>());
            break;
        }
    }
    return custom_data;
}

celeritas::custom_data celeritas::custom_data::from_treasure_description(const document_type& document)
{
    custom_data custom_data{};
    for (const auto& element : document)
    {
        if (element.get_field_name() == data_description)
        {
            custom_data.detail_ = treasure_data::from_document(element.get_value<database_data_type::document_type>());
            break;
        }
    }
    return custom_data;
}

celeritas::custom_data::variant_type celeritas::custom_data::get_variant_type(const std::string_view type)
{
    if (type == custom_description)
    {
        return std::monostate{};
    }

    if (type == consumable_description)
    {
        return consumable_data{};
    }

    if (type == equipment_description)
    {
        return equipment_data{};
    }

    if (type == avatar_description)
    {
        return avatar_data{};
    }

    if (type == frame_description)
    {
        return frame_data{};
    }

    if (type == title_description)
    {
        return title_data{};
    }

    if (type == hero_description)
    {
        return hero_data{};
    }

    if (type == building_description)
    {
        return building_data{};
    }

    if (type == resource_description)
    {
        return resource_data{};
    }

    if (type == soldier_description)
    {
        return soldier_data{};
    }

    if (type == machine_description)
    {
        return machine_data{};
    }

    if (type == skill_book_description)
    {
        return skill_book_data{};
    }

    if (type == blueprint_description)
    {
        return blueprint_data{};
    }

    if (type == gift_box_description)
    {
        return gift_box_data{};
    }

    if (type == treasure_description)
    {
        return treasure_data{};
    }

    throw celeritas_error{ "custom_data type failed." };
}

