// 此文件是自动生成，请勿手动修改。

#include "player_server_roles.h"
#include "config/database_type.h"
#include "database/database_change_type.h"
#include "database/database_entity_change.tpp"
#include "database/database_entity.tpp"
#include "database/entity.tpp"

celeritas::player_server_roles::player_server_roles(const database_entity_change& entity)
    : base_type{ entity },
      _id_{ entity.get_value<database_data_type::int64_type>(entity.get_database_type() == database_type::mongo ? "_id" : _id_describe) },
      servers_{ entity.get_value<database_data_type::document_array_type>(servers_describe) },
      update_time_{ entity.get_value<database_data_type::int64_type>(update_time_describe) }
{
}

celeritas::player_server_roles::player_server_roles(const database_type database_type, traits::param_type::int64_type _id)
    : base_type{ database_type, database_name, get_key_basis_database_container(database_type, _id) },
      _id_{ _id },
      servers_{ traits::document_array_type{} },
      update_time_{ traits::int64_type{} }
{
    add_modify(_id_describe, _id);
}

celeritas::traits::int64_type celeritas::player_server_roles::get__id() const noexcept
{
    return _id_.get_value();
}

celeritas::traits::document_array_type celeritas::player_server_roles::get_servers() const
{
    return servers_.get_value();
}

celeritas::traits::int64_type celeritas::player_server_roles::get_update_time() const noexcept
{
    return update_time_.get_value();
}

void celeritas::player_server_roles::set__id(traits::param_type::int64_type _id)
{
    if (_id != get__id())
    {
        _id_.set_value(_id);

        add_modify(_id_describe, get__id());
    }
}

void celeritas::player_server_roles::set_servers(traits::param_type::document_array_type servers)
{
    if (servers != get_servers())
    {
        servers_.set_value(servers);

        add_modify(servers_describe, get_servers());
    }
}

void celeritas::player_server_roles::set_update_time(traits::param_type::int64_type update_time)
{
    if (update_time != get_update_time())
    {
        update_time_.set_value(update_time);

        add_modify(update_time_describe, get_update_time());
    }
}

const celeritas::database_entity::database_field_container& celeritas::player_server_roles::get_database_field_container()
{
    static const database_field_container field_name_container{ decltype(_id_)::get_database_field(),
                                                                decltype(servers_)::get_database_field(),
                                                                decltype(update_time_)::get_database_field() };

    return field_name_container;
}

celeritas::player_server_roles::database_entity_change_const_shared_ptr celeritas::player_server_roles::get_select(const database_type database_type)
{
    static const auto result = std::make_shared<database_entity_change>(database_type,
                                                                        database_name,
                                                                        database_change_type::select_type);

    return result;
}

celeritas::player_server_roles::database_entity_change_const_shared_ptr celeritas::player_server_roles::get_select(const database_type database_type, traits::param_type::int64_type _id)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    get_key_basis_database_container(database_type, _id));
}

celeritas::player_server_roles::database_entity_change_const_shared_ptr celeritas::player_server_roles::get_select(const database_type database_type, const basis_database_container_const_shared_ptr& key)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    key);
}

celeritas::player_server_roles::basis_database_container_const_shared_ptr celeritas::player_server_roles::get_key_basis_database_container(const database_type database_type, traits::param_type::int64_type _id)
{
    const auto field_name = database_type == database_type::mongo ? "_id" : _id_describe;

    const auto container = std::make_shared<basis_database_container>(basis_database{ field_name, _id });

    return container;
}