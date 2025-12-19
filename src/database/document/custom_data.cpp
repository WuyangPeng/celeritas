#include "custom_data.h"

#include <bsoncxx/types.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>

celeritas::custom_data::custom_data()
    : detail_{ std::monostate{} }
{
}

celeritas::custom_data::document_type celeritas::custom_data::to_document_type() const
{
    document_type builder;
    std::visit(
        [&builder](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, equipment_data>)
            {
                builder.append(bsoncxx::builder::basic::kvp("type", "equipment"));
                builder.append(bsoncxx::builder::basic::kvp("strength", arg.get_strength()));
                builder.append(bsoncxx::builder::basic::kvp("durability", arg.get_durability()));
            }
            else if constexpr (std::is_same_v<T, consumable_data>)
            {
                builder.append(bsoncxx::builder::basic::kvp("type", "consumable"));
                builder.append(bsoncxx::builder::basic::kvp("expire_time", arg.get_expire_time()));
            }
            else if constexpr (std::is_same_v<T, std::monostate>)
            {
                // monostate results in an empty document
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

    auto type_element = document_view["type"];
    if (!type_element || type_element.type() != bsoncxx::type::k_string)
    {
        return;
    }

    auto type_str = type_element.get_string().value;

    if (type_str == "equipment")
    {
        auto strength_element = document_view["strength"];
        auto durability_element = document_view["durability"];

        if (strength_element && (strength_element.type() == bsoncxx::type::k_int32) &&
            durability_element && (durability_element.type() == bsoncxx::type::k_int32))
        {
            detail_ = equipment_data{ strength_element.get_int32().value, durability_element.get_int32().value };
        }
    }
    else if (type_str == "consumable")
    {
        auto expire_element = document_view["expire_time"];
        if (expire_element && (expire_element.type() == bsoncxx::type::k_int64))
        {
            detail_ = consumable_data{ expire_element.get_int64().value };
        }
    }
}
