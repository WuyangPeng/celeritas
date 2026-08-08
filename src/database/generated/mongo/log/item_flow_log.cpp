// 此文件是自动生成，请勿手动修改。

#include "item_flow_log.h"
#include "config/basic/database_type.h"
#include "database/basic/basis_database.tpp"
#include "database/basic/database_change_type.h"
#include "database/basic/database_entity_change.tpp"
#include "database/entity/database_entity.h"
#include "database/entity/entity.tpp"

celeritas::item_flow_log::item_flow_log(const database_entity_change& entity)
    : base_type{ entity.get_database_entity_change(id_describe)  },
      id_{ entity.get_value<database_data_type::int64_type>(id_describe) },
      header_{ entity.get_value<database_data_type::document_type>(header_describe) },
      item_flow_{ entity.get_value<database_data_type::document_type>(item_flow_describe) }
{
}

celeritas::item_flow_log::item_flow_log(const database_type database_type, const database_entity_change& entity)
    : base_type{ database_type, entity.get_database_entity_change(id_describe) },
      id_{ entity.get_value<database_data_type::int64_type>(id_describe) },
      header_{ entity.get_value<database_data_type::document_type>(header_describe) },
      item_flow_{ entity.get_value<database_data_type::document_type>(item_flow_describe) }
{
    if (database_type != entity.get_database_type())
    {
        add_modify(id_describe, get_id());
        add_modify(header_describe, get_header());
        add_modify(item_flow_describe, get_item_flow());
    }
}

celeritas::item_flow_log::item_flow_log(const database_type database_type, traits::param_type::int64_type id)
    : base_type{ database_type, database_name, get_key_basis_database_container(id) },
      id_{ id },
      header_{ traits::document_type{} },
      item_flow_{ traits::document_type{} }
{
    add_modify(id_describe, id);
}

celeritas::traits::int64_type celeritas::item_flow_log::get_id() const noexcept
{
    return id_.get_value();
}

celeritas::traits::document_type celeritas::item_flow_log::get_header() const
{
    return header_.get_value();
}

celeritas::traits::document_type celeritas::item_flow_log::get_item_flow() const
{
    return item_flow_.get_value();
}

void celeritas::item_flow_log::set_id(traits::param_type::int64_type id)
{
    if (id != get_id())
    {
        id_.set_value(id);

        add_modify(id_describe, get_id());
    }
}

void celeritas::item_flow_log::set_header(traits::param_type::document_type header)
{
    if (header != get_header())
    {
        header_.set_value(header);

        add_modify(header_describe, get_header());
    }
}

void celeritas::item_flow_log::set_item_flow(traits::param_type::document_type item_flow)
{
    if (item_flow != get_item_flow())
    {
        item_flow_.set_value(item_flow);

        add_modify(item_flow_describe, get_item_flow());
    }
}

const celeritas::database_entity::database_field_container& celeritas::item_flow_log::get_database_field_container()
{
    static const database_field_container field_name_container{ decltype(id_)::get_database_field(),
                                                                decltype(header_)::get_database_field(),
                                                                decltype(item_flow_)::get_database_field() };

    return field_name_container;
}

celeritas::item_flow_log::const_database_entity_change_shared_ptr celeritas::item_flow_log::get_select(const database_type database_type)
{
    static const auto result = std::make_shared<database_entity_change>(database_type,
                                                                        database_name,
                                                                        database_change_type::select_type);

    return result;
}

celeritas::item_flow_log::const_database_entity_change_shared_ptr celeritas::item_flow_log::get_select(const database_type database_type, traits::param_type::int64_type id)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    get_key_basis_database_container(id));
}

celeritas::item_flow_log::const_database_entity_change_shared_ptr celeritas::item_flow_log::get_select(const database_type database_type, const const_basis_database_container_shared_ptr& key)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    key);
}

celeritas::item_flow_log::const_basis_database_container_shared_ptr celeritas::item_flow_log::get_key_basis_database_container(traits::param_type::int64_type id)
{
    return std::make_shared<basis_database_container>(basis_database{ id_describe, id });
}