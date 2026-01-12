// 此文件是自动生成，请勿手动修改。

#include "account_last_login.h"
#include "config/basic/database_type.h"
#include "database/basic/basis_database.tpp"
#include "database/basic/database_change_type.h"
#include "database/basic/database_entity_change.tpp"
#include "database/entity/database_entity.h"
#include "database/entity/entity.tpp"

celeritas::account_last_login::account_last_login(const database_entity_change& entity)
    : base_type{ entity },
      account_id_{ entity.get_value<database_data_type::int64_type>(account_id_describe) },
      game_server_id_{ entity.get_value<database_data_type::string_type>(game_server_id_describe) },
      update_time_{ entity.get_value<database_data_type::int64_type>(update_time_describe) }
{
}

celeritas::account_last_login::account_last_login(const database_type database_type, const database_entity_change& entity)
    : base_type{ database_type, entity },
      account_id_{ entity.get_value<database_data_type::int64_type>(account_id_describe) },
      game_server_id_{ entity.get_value<database_data_type::string_type>(game_server_id_describe) },
      update_time_{ entity.get_value<database_data_type::int64_type>(update_time_describe) }
{
    if (database_type != entity.get_database_type())
    {
        add_modify(account_id_describe, get_account_id());
        add_modify(game_server_id_describe, get_game_server_id());
        add_modify(update_time_describe, get_update_time());
    }
}

celeritas::account_last_login::account_last_login(const database_type database_type, traits::param_type::int64_type account_id)
    : base_type{ database_type, database_name, get_key_basis_database_container(account_id) },
      account_id_{ account_id },
      game_server_id_{ traits::string_type{} },
      update_time_{ traits::int64_type{} }
{
    add_modify(account_id_describe, account_id);
}

celeritas::traits::int64_type celeritas::account_last_login::get_account_id() const noexcept
{
    return account_id_.get_value();
}

celeritas::traits::string_type celeritas::account_last_login::get_game_server_id() const
{
    return game_server_id_.get_value();
}

celeritas::traits::int64_type celeritas::account_last_login::get_update_time() const noexcept
{
    return update_time_.get_value();
}

void celeritas::account_last_login::set_account_id(traits::param_type::int64_type account_id)
{
    if (account_id != get_account_id())
    {
        account_id_.set_value(account_id);

        add_modify(account_id_describe, get_account_id());
    }
}

void celeritas::account_last_login::set_game_server_id(traits::param_type::string_type game_server_id)
{
    if (game_server_id != get_game_server_id())
    {
        game_server_id_.set_value(game_server_id);

        add_modify(game_server_id_describe, get_game_server_id());
    }
}

void celeritas::account_last_login::set_update_time(traits::param_type::int64_type update_time)
{
    if (update_time != get_update_time())
    {
        update_time_.set_value(update_time);

        add_modify(update_time_describe, get_update_time());
    }
}

const celeritas::database_entity::database_field_container& celeritas::account_last_login::get_database_field_container()
{
    static const database_field_container field_name_container{ decltype(account_id_)::get_database_field(),
                                                                decltype(game_server_id_)::get_database_field(),
                                                                decltype(update_time_)::get_database_field() };

    return field_name_container;
}

celeritas::account_last_login::const_database_entity_change_shared_ptr celeritas::account_last_login::get_select(const database_type database_type)
{
    static const auto result = std::make_shared<database_entity_change>(database_type,
                                                                        database_name,
                                                                        database_change_type::select_type);

    return result;
}

celeritas::account_last_login::const_database_entity_change_shared_ptr celeritas::account_last_login::get_select(const database_type database_type, traits::param_type::int64_type account_id)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    get_key_basis_database_container(account_id));
}

celeritas::account_last_login::const_database_entity_change_shared_ptr celeritas::account_last_login::get_select(const database_type database_type, const const_basis_database_container_shared_ptr& key)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    key);
}

celeritas::account_last_login::const_basis_database_container_shared_ptr celeritas::account_last_login::get_key_basis_database_container(traits::param_type::int64_type account_id)
{
    return std::make_shared<basis_database_container>(basis_database{ account_id_describe, account_id });
}