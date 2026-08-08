// 此文件是自动生成，请勿手动修改。

#pragma once

#include "database/basic/database_data_type_traits.h"
#include "database/basic/database_index_type.h"
#include "database/entity/database_entity.h"
#include "database/entity/entity.h"

namespace celeritas
{
    class item_flow_log final : public database_entity
    {
    public:
        using class_type = item_flow_log;
        using base_type = database_entity;

        explicit item_flow_log(const database_entity_change& entity);

        item_flow_log(database_type database_type, const database_entity_change& entity);

        item_flow_log(database_type database_type, traits::param_type::int64_type id);

        [[nodiscard]] traits::int64_type get_id() const noexcept;

        [[nodiscard]] traits::document_type get_header() const;

        [[nodiscard]] traits::document_type get_item_flow() const;

        void set_id(traits::param_type::int64_type id);

        void set_header(traits::param_type::document_type header);

        void set_item_flow(traits::param_type::document_type item_flow);

        [[nodiscard]] static const database_field_container& get_database_field_container();

        [[nodiscard]] static const_database_entity_change_shared_ptr get_select(database_type database_type);

        [[nodiscard]] static const_database_entity_change_shared_ptr get_select(database_type database_type, traits::param_type::int64_type id);

        [[nodiscard]] static const_database_entity_change_shared_ptr get_select(database_type database_type, const const_basis_database_container_shared_ptr& key);

        static constexpr std::string_view database_name{ "item_flow_log" };

        static constexpr std::string_view id_describe{ "_id" };
        static constexpr std::string_view header_describe{ "header" };
        static constexpr std::string_view item_flow_describe{ "item_flow" };

    private:
        [[nodiscard]] static const_basis_database_container_shared_ptr get_key_basis_database_container(traits::param_type::int64_type id);

        entity<id_describe, database_data_type::int64_type, database_index_type::key> id_;
        entity<header_describe, database_data_type::document_type> header_;
        entity<item_flow_describe, database_data_type::document_type> item_flow_;
    };
}