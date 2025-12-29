#include "custom_data.h"
#include "develop_data.h"
#include "database/basis_database.tpp"

#include <bsoncxx/types.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>

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
    custom_data custom_data{};

    std::string type{};
    for (const auto& element : document)
    {
        if (element.get_field_name() == type_description)
        {
            type = element.get_value<database_data_type::string_type>();
            break;
        }
    }

    if (type == equipment_description)
    {
        for (const auto& element : document)
        {
            if (element.get_field_name() == data_description)
            {
                custom_data.detail_ = equipment_data::from_document(element.get_value<database_data_type::document_type>());
                break;
            }
        }
    }
    else if (type == consumable_description)
    {
        for (const auto& element : document)
        {
            if (element.get_field_name() == data_description)
            {
                custom_data.detail_ = consumable_data::from_document(element.get_value<database_data_type::document_type>());
                break;
            }
        }
    }

    return custom_data;
}