#include "custom_data.h"

#include <bsoncxx/types.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>

celeritas::custom_data::custom_data()
    : detail_{ std::monostate{} }
{
}

celeritas::custom_data::document_type celeritas::custom_data::to_document_type() const
{
    document_type builder{};
    std::visit(
        [&builder]<typename ArgType>(ArgType&& arg) {
            using T = std::decay_t<ArgType>;
            if constexpr (std::is_same_v<T, equipment_data>)
            {
                builder.append(bsoncxx::builder::basic::kvp(std::string{ type_description }, std::string{ equipment_description }));
                builder.append(bsoncxx::builder::basic::kvp(std::string{ data_description }, arg.to_document_type().view()));
            }
            else if constexpr (std::is_same_v<T, consumable_data>)
            {
                builder.append(bsoncxx::builder::basic::kvp(std::string{ type_description }, std::string{ consumable_description }));
                builder.append(bsoncxx::builder::basic::kvp(std::string{ data_description }, arg.to_document_type().view()));
            }
            else if constexpr (std::is_same_v<T, std::monostate>)
            {
                // monostate结果为空文档
            }
        },
        detail_);
    return builder;
}

void celeritas::custom_data::set_document(const document_view_type& document_view)
{
    if (document_view.empty())
    {
        return;
    }

    const auto type_element = document_view[type_description];
    if (!type_element || type_element.type() != bsoncxx::type::k_string)
    {
        return;
    }

    const auto type = type_element.get_string().value;
    const auto data_element = document_view[data_description];
    if (!data_element || data_element.type() != bsoncxx::type::k_document)
    {
        return;
    }

    const auto data_view = data_element.get_document().value;
    if (type == equipment_description)
    {
        detail_ = equipment_data{ data_view };
    }
    else if (type == consumable_description)
    {
        detail_ = consumable_data{ data_view };
    }
}
