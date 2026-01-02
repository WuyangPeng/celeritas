// 此文件是自动生成，请勿手动修改。

#include "user_server_roles.h"
#include "config/basic/database_type.h"
#include "database/database_change_type.h"
#include "database/database_entity_change.tpp"
#include "database/database_entity.tpp"
#include "database/entity.tpp"

celeritas::user_server_roles::user_server_roles(const database_entity_change& entity)
    : base_type{ entity },
      id_{ entity.get_value<database_data_type::int64_type>(id_describe) },
      servers_{ entity.get_value<database_data_type::document_array_type>(servers_describe) },
      update_time_{ entity.get_value<database_data_type::int64_type>(update_time_describe) }
{
}

celeritas::user_server_roles::user_server_roles(const database_type database_type, const database_entity_change& entity)
    : base_type{ database_type, entity },
      id_{ entity.get_value<database_data_type::int64_type>(id_describe) },
      servers_{ entity.get_value<database_data_type::document_array_type>(servers_describe) },
      update_time_{ entity.get_value<database_data_type::int64_type>(update_time_describe) }
{
    if (database_type != entity.get_database_type())
    {
        add_modify(id_describe, get_id());
        add_modify(servers_describe, get_servers());
        add_modify(update_time_describe, get_update_time());
    }
}

celeritas::user_server_roles::user_server_roles(const database_type database_type, traits::param_type::int64_type id)
    : base_type{ database_type, database_name, get_key_basis_database_container(id) },
      id_{ id },
      servers_{ traits::document_array_type{} },
      update_time_{ traits::int64_type{} }
{
    add_modify(id_describe, id);
}

celeritas::traits::int64_type celeritas::user_server_roles::get_id() const noexcept
{
    return id_.get_value();
}

celeritas::traits::document_array_type celeritas::user_server_roles::get_servers() const
{
    return servers_.get_value();
}

celeritas::traits::int64_type celeritas::user_server_roles::get_update_time() const noexcept
{
    return update_time_.get_value();
}

void celeritas::user_server_roles::set_id(traits::param_type::int64_type id)
{
    if (id != get_id())
    {
        id_.set_value(id);

        add_modify(id_describe, get_id());
    }
}

void celeritas::user_server_roles::set_servers(traits::param_type::document_array_type servers)
{
    if (servers != get_servers())
    {
        servers_.set_value(servers);

        add_modify(servers_describe, get_servers());
    }
}

void celeritas::user_server_roles::set_update_time(traits::param_type::int64_type update_time)
{
    if (update_time != get_update_time())
    {
        update_time_.set_value(update_time);

        add_modify(update_time_describe, get_update_time());
    }
}

void celeritas::user_server_roles::set_servers(const int index, traits::param_type::document_array_element_type servers)
{
    if (servers_.set_value(index, servers))
    {
        add_modify(servers_describe, get_servers());
    }
}

void celeritas::user_server_roles::add_servers(traits::param_type::document_array_element_type servers)
{
    servers_.add_value(servers);

    add_modify(servers_describe, get_servers());
}

void celeritas::user_server_roles::remove_servers(const int index)
{
    servers_.remove_value(index);

    add_modify(servers_describe, get_servers());
}

const celeritas::database_entity::database_field_container& celeritas::user_server_roles::get_database_field_container()
{
    static const database_field_container field_name_container{ decltype(id_)::get_database_field(),
                                                                decltype(servers_)::get_database_field(),
                                                                decltype(update_time_)::get_database_field() };

    return field_name_container;
}

celeritas::user_server_roles::database_entity_change_const_shared_ptr celeritas::user_server_roles::get_select(const database_type database_type)
{
    static const auto result = std::make_shared<database_entity_change>(database_type,
                                                                        database_name,
                                                                        database_change_type::select_type);

    return result;
}

celeritas::user_server_roles::database_entity_change_const_shared_ptr celeritas::user_server_roles::get_select(const database_type database_type, traits::param_type::int64_type id)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    get_key_basis_database_container(id));
}

celeritas::user_server_roles::database_entity_change_const_shared_ptr celeritas::user_server_roles::get_select(const database_type database_type, const basis_database_container_const_shared_ptr& key)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    key);
}

celeritas::user_server_roles::basis_database_container_const_shared_ptr celeritas::user_server_roles::get_key_basis_database_container(traits::param_type::int64_type id)
{
    return std::make_shared<basis_database_container>(basis_database{ id_describe, id });
}