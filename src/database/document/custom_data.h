#pragma once

#include "consumable_data.h"
#include "equipment_data.h"

#include <bsoncxx/builder/basic/document.hpp>
#include <variant>

namespace celeritas
{
    class custom_data
    {
    public:
        using class_type = custom_data;
        using document_type = bsoncxx::builder::basic::document;
        using document_view_type = bsoncxx::document::view;

        custom_data();

        [[nodiscard]] document_type to_document_type() const;

        void set_document(const document_view_type& document_view);

    private:
        using variant_type = std::variant<std::monostate, equipment_data, consumable_data>;

        variant_type detail_;
    };
}