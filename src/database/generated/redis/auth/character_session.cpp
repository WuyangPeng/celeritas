// 此文件是自动生成，请勿手动修改。

#include "character_session.h"
#include "config/database_type.h"
#include "database/database_change_type.h"
#include "database/database_entity_change.tpp"
#include "database/database_entity.tpp"
#include "database/entity.tpp"

celeritas::character_session::character_session(const database_entity_change& entity)
    : base_type{ entity },
      account_id_{ entity.get_value<database_data_type::int64_type>(entity.get_database_type() == database_type::mongo ? "_id" : account_id_describe) },
      player_server_instance_id_{ entity.get_value<database_data_type::string_type>(player_server_instance_id_describe) }
{
}

celeritas::character_session::character_session(const database_type database_type, const database_entity_change& entity)
    : base_type{ database_type, entity },
      account_id_{ entity.get_value<database_data_type::int64_type>(entity.get_database_type() == database_type::mongo ? "_id" : account_id_describe) },
      player_server_instance_id_{ entity.get_value<database_data_type::string_type>(player_server_instance_id_describe) }
{
    if(database_type != entity.get_database_type())
    {
        
    }
}

celeritas::character_session::character_session(const database_type database_type, traits::param_type::int64_type account_id)
    : base_type{ database_type, database_name, get_key_basis_database_container(database_type, account_id) },
      account_id_{ account_id },
      player_server_instance_id_{ traits::string_type{} }
{
    add_modify(account_id_describe, account_id);
}

celeritas::traits::int64_type celeritas::character_session::get_account_id() const noexcept
{
    return account_id_.get_value();
}

celeritas::traits::string_type celeritas::character_session::get_player_server_instance_id() const
{
    return player_server_instance_id_.get_value();
}

void celeritas::character_session::set_account_id(traits::param_type::int64_type account_id)
{
    if (account_id != get_account_id())
    {
        account_id_.set_value(account_id);

        add_modify(account_id_describe, get_account_id());
    }
}

void celeritas::character_session::set_player_server_instance_id(traits::param_type::string_type player_server_instance_id)
{
    if (player_server_instance_id != get_player_server_instance_id())
    {
        player_server_instance_id_.set_value(player_server_instance_id);

        add_modify(player_server_instance_id_describe, get_player_server_instance_id());
    }
}

const celeritas::database_entity::database_field_container& celeritas::character_session::get_database_field_container()
{
    static const database_field_container field_name_container{ decltype(account_id_)::get_database_field(),
                                                                decltype(player_server_instance_id_)::get_database_field() };

    return field_name_container;
}

celeritas::character_session::database_entity_change_const_shared_ptr celeritas::character_session::get_select(const database_type database_type)
{
    static const auto result = std::make_shared<database_entity_change>(database_type,
                                                                        database_name,
                                                                        database_change_type::select_type);

    return result;
}

celeritas::character_session::database_entity_change_const_shared_ptr celeritas::character_session::get_select(const database_type database_type, traits::param_type::int64_type account_id)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    get_key_basis_database_container(database_type, account_id));
}

celeritas::character_session::database_entity_change_const_shared_ptr celeritas::character_session::get_select(const database_type database_type, const basis_database_container_const_shared_ptr& key)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    key);
}

celeritas::character_session::basis_database_container_const_shared_ptr celeritas::character_session::get_key_basis_database_container(const database_type database_type, traits::param_type::int64_type account_id)
{
    const auto field_name = database_type == database_type::mongo ? "_id" : account_id_describe;

    const auto container = std::make_shared<basis_database_container>(basis_database{ field_name, account_id });

    return container;
}