#pragma once

#include "custom_data.h"
#include "config/luban/generated/schema.h"
#include "database/basic/database_data_type_traits.h"

#include <cstdint>

namespace celeritas
{
    class item_selected_data
    {
    public:
        using class_type = item_selected_data;
        using document_type = traits::document_type;

        item_selected_data() noexcept = default;

        item_selected_data(int64_t id,
                           config::item_type item_type,
                           config::item_selected_child_type child_type,
                           int64_t operation_id,
                           int parameter,
                           int64_t selected_id);

        [[nodiscard]] document_type to_document_type() const;

        [[nodiscard]] static item_selected_data from_document(const document_type& document);

        static constexpr std::string_view id_description = "id";
        static constexpr std::string_view item_type_description = "item_type";
        static constexpr std::string_view child_type_description = "child_type";
        static constexpr std::string_view operation_id_description = "operation_id";
        static constexpr std::string_view parameter_description = "parameter";
        static constexpr std::string_view selected_id_description = "selected_id";

        [[nodiscard]] int64_t get_id() const;

        void set_id(int64_t id);

        [[nodiscard]] config::item_type get_item_type() const;

        void set_item_type(config::item_type itemType);

        [[nodiscard]] config::item_selected_child_type get_child_type() const;

        void set_child_type(config::item_selected_child_type childType);

        [[nodiscard]] int64_t get_selected_id() const;

        void set_selected_id(int64_t selectedId);

        [[nodiscard]] int64_t get_operation_id() const;

        void set_operation_id(int64_t operationId);

        [[nodiscard]] int get_parameter() const;

        void set_parameter(int parameter);

    private:
        int64_t id_ = 0;
        config::item_type item_type_ = config::item_type::none;
        config::item_selected_child_type child_type_ = config::item_selected_child_type::none;
        int64_t operation_id_ = 0;
        int parameter_ = 0;
        int64_t selected_id_ = 0;
    };
}