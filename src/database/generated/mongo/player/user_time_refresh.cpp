// 此文件是自动生成，请勿手动修改。

#include "user_time_refresh.h"
#include "config/database_type.h"
#include "database/database_change_type.h"
#include "database/database_entity_change.tpp"
#include "database/database_entity.tpp"
#include "database/entity.tpp"

celeritas::user_time_refresh::user_time_refresh(const database_entity_change& entity)
    : base_type{ entity },
      user_id_{ entity.get_value<database_data_type::int64_type>(entity.get_database_type() == database_type::mongo ? "_id" : user_id_describe) },
      player_time_refresh_{ entity.get_value<database_data_type::document_array_type>(player_time_refresh_describe) }
{
}

celeritas::user_time_refresh::user_time_refresh(const database_type database_type, const database_entity_change& entity)
    : base_type{ database_type, entity },
      user_id_{ entity.get_value<database_data_type::int64_type>(entity.get_database_type() == database_type::mongo ? "_id" : user_id_describe) },
      player_time_refresh_{ entity.get_value<database_data_type::document_array_type>(player_time_refresh_describe) }
{
    if (database_type != entity.get_database_type())
    {
        add_modify(user_id_describe, get_user_id());
        add_modify(player_time_refresh_describe, get_player_time_refresh());
    }
}

celeritas::user_time_refresh::user_time_refresh(const database_type database_type, traits::param_type::int64_type user_id)
    : base_type{ database_type, database_name, get_key_basis_database_container(database_type, user_id) },
      user_id_{ user_id },
      player_time_refresh_{ traits::document_array_type{} }
{
    add_modify(user_id_describe, user_id);
}

celeritas::traits::int64_type celeritas::user_time_refresh::get_user_id() const noexcept
{
    return user_id_.get_value();
}

celeritas::traits::document_array_type celeritas::user_time_refresh::get_player_time_refresh() const
{
    return player_time_refresh_.get_value();
}

void celeritas::user_time_refresh::set_user_id(traits::param_type::int64_type user_id)
{
    if (user_id != get_user_id())
    {
        user_id_.set_value(user_id);

        add_modify(user_id_describe, get_user_id());
    }
}

void celeritas::user_time_refresh::set_player_time_refresh(traits::param_type::document_array_type player_time_refresh)
{
    if (player_time_refresh != get_player_time_refresh())
    {
        player_time_refresh_.set_value(player_time_refresh);

        add_modify(player_time_refresh_describe, get_player_time_refresh());
    }
}

void celeritas::user_time_refresh::set_player_time_refresh(const int index, traits::param_type::document_array_element_type player_time_refresh)
{
    if (player_time_refresh_.set_value(index, player_time_refresh))
    {
        add_modify(player_time_refresh_describe, get_player_time_refresh());
    }
}

void celeritas::user_time_refresh::add_player_time_refresh(traits::param_type::document_array_element_type player_time_refresh)
{
    player_time_refresh_.add_value(player_time_refresh);

    add_modify(player_time_refresh_describe, get_player_time_refresh());
}

void celeritas::user_time_refresh::remove_player_time_refresh(const int index)
{
    player_time_refresh_.remove_value(index);

    add_modify(player_time_refresh_describe, get_player_time_refresh());
}

const celeritas::database_entity::database_field_container& celeritas::user_time_refresh::get_database_field_container()
{
    static const database_field_container field_name_container{ decltype(user_id_)::get_database_field(),
                                                                decltype(player_time_refresh_)::get_database_field() };

    return field_name_container;
}

const celeritas::database_entity::database_field_container& celeritas::user_time_refresh::get_mongo_database_field_container()
{
    static const database_field_container field_name_container{ decltype(user_id_)::get_mongo_database_field(),
                                                                decltype(player_time_refresh_)::get_database_field() };

    return field_name_container;
}

celeritas::user_time_refresh::database_entity_change_const_shared_ptr celeritas::user_time_refresh::get_select(const database_type database_type)
{
    static const auto result = std::make_shared<database_entity_change>(database_type,
                                                                        database_name,
                                                                        database_change_type::select_type);

    return result;
}

celeritas::user_time_refresh::database_entity_change_const_shared_ptr celeritas::user_time_refresh::get_select(const database_type database_type, traits::param_type::int64_type user_id)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    get_key_basis_database_container(database_type, user_id));
}

celeritas::user_time_refresh::database_entity_change_const_shared_ptr celeritas::user_time_refresh::get_select(const database_type database_type, const basis_database_container_const_shared_ptr& key)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    key);
}

celeritas::user_time_refresh::basis_database_container_const_shared_ptr celeritas::user_time_refresh::get_key_basis_database_container(const database_type database_type, traits::param_type::int64_type user_id)
{
    const auto field_name = database_type == database_type::mongo ? "_id" : user_id_describe;

    const auto container = std::make_shared<basis_database_container>(basis_database{ field_name, user_id });

    return container;
}