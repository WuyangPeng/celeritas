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
            if constexpr (std::is_same_v<T, equipment_data>)
            {
                document.emplace_back(type_description, std::string{ equipment_description });
                document.emplace_back(data_description, arg.to_document_type());
            }
            else if constexpr (std::is_same_v<T, consumable_data>)
            {
                document.emplace_back(type_description, std::string{ consumable_description });
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

    if (type == equipment_description)
    {
        return from_equipment_description(document);
    }

    if (type == consumable_description)
    {
        return from_consumable_description(document);
    }

    throw celeritas_error{ "custom_data::from_document() failed." };
}

celeritas::custom_data::kind celeritas::custom_data::get_kind() const noexcept
{
    if (std::holds_alternative<equipment_data>(detail_))
    {
        return kind::equipment;
    }
    if (std::holds_alternative<consumable_data>(detail_))
    {
        return kind::consumable;
    }
    return kind::none;
}

const celeritas::equipment_data* celeritas::custom_data::get_equipment() const noexcept
{
    return std::get_if<equipment_data>(&detail_);
}

const celeritas::consumable_data* celeritas::custom_data::get_consumable() const noexcept
{
    return std::get_if<consumable_data>(&detail_);
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