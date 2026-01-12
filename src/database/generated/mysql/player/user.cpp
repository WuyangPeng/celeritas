// 此文件是自动生成，请勿手动修改。

#include "user.h"
#include "config/basic/database_type.h"
#include "database/basic/basis_database.tpp"
#include "database/basic/database_change_type.h"
#include "database/basic/database_entity_change.tpp"
#include "database/entity/database_entity.h"
#include "database/entity/entity.tpp"

celeritas::user::user(const database_entity_change& entity)
    : base_type{ entity },
      user_id_{ entity.get_value<database_data_type::int64_type>(user_id_describe) },
      account_id_{ entity.get_value<database_data_type::int64_type>(account_id_describe) },
      game_server_id_{ entity.get_value<database_data_type::string_type>(game_server_id_describe) },
      overload_db_{ entity.get_value<database_data_type::bool_type>(overload_db_describe) }
{
}

celeritas::user::user(const database_type database_type, const database_entity_change& entity)
    : base_type{ database_type, entity },
      user_id_{ entity.get_value<database_data_type::int64_type>(user_id_describe) },
      account_id_{ entity.get_value<database_data_type::int64_type>(account_id_describe) },
      game_server_id_{ entity.get_value<database_data_type::string_type>(game_server_id_describe) },
      overload_db_{ entity.get_value<database_data_type::bool_type>(overload_db_describe) }
{
    if (database_type != entity.get_database_type())
    {
        add_modify(user_id_describe, get_user_id());
        add_modify(account_id_describe, get_account_id());
        add_modify(game_server_id_describe, get_game_server_id());
        add_modify(overload_db_describe, is_overload_db());
    }
}

celeritas::user::user(const database_type database_type, traits::param_type::int64_type user_id)
    : base_type{ database_type, database_name, get_key_basis_database_container(user_id) },
      user_id_{ user_id },
      account_id_{ traits::int64_type{} },
      game_server_id_{ traits::string_type{} },
      overload_db_{ traits::bool_type{} }
{
    add_modify(user_id_describe, user_id);
}

celeritas::traits::int64_type celeritas::user::get_user_id() const noexcept
{
    return user_id_.get_value();
}

celeritas::traits::int64_type celeritas::user::get_account_id() const noexcept
{
    return account_id_.get_value();
}

celeritas::traits::string_type celeritas::user::get_game_server_id() const
{
    return game_server_id_.get_value();
}

celeritas::traits::bool_type celeritas::user::is_overload_db() const noexcept
{
    return overload_db_.get_value();
}

void celeritas::user::set_user_id(traits::param_type::int64_type user_id)
{
    if (user_id != get_user_id())
    {
        user_id_.set_value(user_id);

        add_modify(user_id_describe, get_user_id());
    }
}

void celeritas::user::set_account_id(traits::param_type::int64_type account_id)
{
    if (account_id != get_account_id())
    {
        account_id_.set_value(account_id);

        add_modify(account_id_describe, get_account_id());
    }
}

void celeritas::user::set_game_server_id(traits::param_type::string_type game_server_id)
{
    if (game_server_id != get_game_server_id())
    {
        game_server_id_.set_value(game_server_id);

        add_modify(game_server_id_describe, get_game_server_id());
    }
}

void celeritas::user::set_overload_db(traits::param_type::bool_type overload_db)
{
    if (overload_db != is_overload_db())
    {
        overload_db_.set_value(overload_db);

        add_modify(overload_db_describe, is_overload_db());
    }
}

const celeritas::database_entity::database_field_container& celeritas::user::get_database_field_container()
{
    static const database_field_container field_name_container{ decltype(user_id_)::get_database_field(),
                                                                decltype(account_id_)::get_database_field(),
                                                                decltype(game_server_id_)::get_database_field(),
                                                                decltype(overload_db_)::get_database_field() };

    return field_name_container;
}

celeritas::user::const_database_entity_change_shared_ptr celeritas::user::get_select(const database_type database_type)
{
    static const auto result = std::make_shared<database_entity_change>(database_type,
                                                                        database_name,
                                                                        database_change_type::select_type);

    return result;
}

celeritas::user::const_database_entity_change_shared_ptr celeritas::user::get_select(const database_type database_type, traits::param_type::int64_type user_id)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    get_key_basis_database_container(user_id));
}

celeritas::user::const_database_entity_change_shared_ptr celeritas::user::get_select(const database_type database_type, const const_basis_database_container_shared_ptr& key)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    key);
}

celeritas::user::const_basis_database_container_shared_ptr celeritas::user::get_key_basis_database_container(traits::param_type::int64_type user_id)
{
    return std::make_shared<basis_database_container>(basis_database{ user_id_describe, user_id });
}