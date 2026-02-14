#include "custom_data.h"
#include "common/core/celeritas_error.h"
#include "database/basic/basis_database.tpp"

celeritas::custom_data::custom_data()
    : detail_{ std::monostate{} }
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

