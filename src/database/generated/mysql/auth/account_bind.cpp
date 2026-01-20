// 此文件是自动生成，请勿手动修改。

#include "account_bind.h"
#include "config/basic/database_type.h"
#include "database/basic/basis_database.tpp"
#include "database/basic/database_change_type.h"
#include "database/basic/database_entity_change.tpp"
#include "database/entity/database_entity.h"
#include "database/entity/entity.tpp"

celeritas::account_bind::account_bind(const database_entity_change& entity)
    : base_type{ entity.get_database_entity_change(account_bind_id_describe)  },
      account_bind_id_{ entity.get_value<database_data_type::int64_type>(account_bind_id_describe) },
      account_id_{ entity.get_value<database_data_type::int64_type>(account_id_describe) },
      account_type_{ entity.get_value<database_data_type::int32_type>(account_type_describe) },
      process_type_{ entity.get_value<database_data_type::int32_type>(process_type_describe) },
      app_id_{ entity.get_value<database_data_type::int64_type>(app_id_describe) },
      auth_key_{ entity.get_value<database_data_type::string_type>(auth_key_describe) }
{
}

celeritas::account_bind::account_bind(const database_type database_type, const database_entity_change& entity)
    : base_type{ database_type, entity.get_database_entity_change(account_bind_id_describe) },
      account_bind_id_{ entity.get_value<database_data_type::int64_type>(account_bind_id_describe) },
      account_id_{ entity.get_value<database_data_type::int64_type>(account_id_describe) },
      account_type_{ entity.get_value<database_data_type::int32_type>(account_type_describe) },
      process_type_{ entity.get_value<database_data_type::int32_type>(process_type_describe) },
      app_id_{ entity.get_value<database_data_type::int64_type>(app_id_describe) },
      auth_key_{ entity.get_value<database_data_type::string_type>(auth_key_describe) }
{
    if (database_type != entity.get_database_type())
    {
        add_modify(account_bind_id_describe, get_account_bind_id());
        add_modify(account_id_describe, get_account_id());
        add_modify(account_type_describe, get_account_type());
        add_modify(process_type_describe, get_process_type());
        add_modify(app_id_describe, get_app_id());
        add_modify(auth_key_describe, get_auth_key());
    }
}

celeritas::account_bind::account_bind(const database_type database_type, traits::param_type::int64_type account_bind_id)
    : base_type{ database_type, database_name, get_key_basis_database_container(account_bind_id) },
      account_bind_id_{ account_bind_id },
      account_id_{ traits::int64_type{} },
      account_type_{ traits::int32_type{} },
      process_type_{ traits::int32_type{} },
      app_id_{ traits::int64_type{} },
      auth_key_{ traits::string_type{} }
{
    add_modify(account_bind_id_describe, account_bind_id);
}

celeritas::traits::int64_type celeritas::account_bind::get_account_bind_id() const noexcept
{
    return account_bind_id_.get_value();
}

celeritas::traits::int64_type celeritas::account_bind::get_account_id() const noexcept
{
    return account_id_.get_value();
}

celeritas::traits::int32_type celeritas::account_bind::get_account_type() const noexcept
{
    return account_type_.get_value();
}

celeritas::traits::int32_type celeritas::account_bind::get_process_type() const noexcept
{
    return process_type_.get_value();
}

celeritas::traits::int64_type celeritas::account_bind::get_app_id() const noexcept
{
    return app_id_.get_value();
}

celeritas::traits::string_type celeritas::account_bind::get_auth_key() const
{
    return auth_key_.get_value();
}

void celeritas::account_bind::set_account_bind_id(traits::param_type::int64_type account_bind_id)
{
    if (account_bind_id != get_account_bind_id())
    {
        account_bind_id_.set_value(account_bind_id);

        add_modify(account_bind_id_describe, get_account_bind_id());
    }
}

void celeritas::account_bind::set_account_id(traits::param_type::int64_type account_id)
{
    if (account_id != get_account_id())
    {
        account_id_.set_value(account_id);

        add_modify(account_id_describe, get_account_id());
    }
}

void celeritas::account_bind::set_account_type(traits::param_type::int32_type account_type)
{
    if (account_type != get_account_type())
    {
        account_type_.set_value(account_type);

        add_modify(account_type_describe, get_account_type());
    }
}

void celeritas::account_bind::set_process_type(traits::param_type::int32_type process_type)
{
    if (process_type != get_process_type())
    {
        process_type_.set_value(process_type);

        add_modify(process_type_describe, get_process_type());
    }
}

void celeritas::account_bind::set_app_id(traits::param_type::int64_type app_id)
{
    if (app_id != get_app_id())
    {
        app_id_.set_value(app_id);

        add_modify(app_id_describe, get_app_id());
    }
}

void celeritas::account_bind::set_auth_key(traits::param_type::string_type auth_key)
{
    if (auth_key != get_auth_key())
    {
        auth_key_.set_value(auth_key);

        add_modify(auth_key_describe, get_auth_key());
    }
}

const celeritas::database_entity::database_field_container& celeritas::account_bind::get_database_field_container()
{
    static const database_field_container field_name_container{ decltype(account_bind_id_)::get_database_field(),
                                                                decltype(account_id_)::get_database_field(),
                                                                decltype(account_type_)::get_database_field(),
                                                                decltype(process_type_)::get_database_field(),
                                                                decltype(app_id_)::get_database_field(),
                                                                decltype(auth_key_)::get_database_field() };

    return field_name_container;
}

celeritas::account_bind::const_database_entity_change_shared_ptr celeritas::account_bind::get_select(const database_type database_type)
{
    static const auto result = std::make_shared<database_entity_change>(database_type,
                                                                        database_name,
                                                                        database_change_type::select_type);

    return result;
}

celeritas::account_bind::const_database_entity_change_shared_ptr celeritas::account_bind::get_select(const database_type database_type, traits::param_type::int64_type account_bind_id)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    get_key_basis_database_container(account_bind_id));
}

celeritas::account_bind::const_database_entity_change_shared_ptr celeritas::account_bind::get_select(const database_type database_type, const const_basis_database_container_shared_ptr& key)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    key);
}

celeritas::account_bind::const_basis_database_container_shared_ptr celeritas::account_bind::get_key_basis_database_container(traits::param_type::int64_type account_bind_id)
{
    return std::make_shared<basis_database_container>(basis_database{ account_bind_id_describe, account_bind_id });
}