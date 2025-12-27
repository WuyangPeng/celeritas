// 此文件是自动生成，请勿手动修改。

#include "account.h"
#include "config/database_type.h"
#include "database/database_change_type.h"
#include "database/database_entity_change.tpp"
#include "database/database_entity.tpp"
#include "database/entity.tpp"

celeritas::account::account(const database_entity_change& entity)
    : base_type{ entity },
      account_id_{ entity.get_value<database_data_type::int64_type>(account_id_describe) },
      account_name_{ entity.get_value<database_data_type::string_type>(account_name_describe) },
      password_hash_{ entity.get_value<database_data_type::string_type>(password_hash_describe) },
      salt_{ entity.get_value<database_data_type::string_type>(salt_describe) },
      device_id_{ entity.get_value<database_data_type::string_type>(device_id_describe) },
      app_id_{ entity.get_value<database_data_type::int64_type>(app_id_describe) },
      create_time_{ entity.get_value<database_data_type::int64_type>(create_time_describe) },
      status_{ entity.get_value<database_data_type::int32_type>(status_describe) }
{
}

celeritas::account::account(const database_type database_type, const database_entity_change& entity)
    : base_type{ database_type, entity },
      account_id_{ entity.get_value<database_data_type::int64_type>(account_id_describe) },
      account_name_{ entity.get_value<database_data_type::string_type>(account_name_describe) },
      password_hash_{ entity.get_value<database_data_type::string_type>(password_hash_describe) },
      salt_{ entity.get_value<database_data_type::string_type>(salt_describe) },
      device_id_{ entity.get_value<database_data_type::string_type>(device_id_describe) },
      app_id_{ entity.get_value<database_data_type::int64_type>(app_id_describe) },
      create_time_{ entity.get_value<database_data_type::int64_type>(create_time_describe) },
      status_{ entity.get_value<database_data_type::int32_type>(status_describe) }
{
    if (database_type != entity.get_database_type())
    {
        add_modify(account_id_describe, get_account_id());
        add_modify(account_name_describe, get_account_name());
        add_modify(password_hash_describe, get_password_hash());
        add_modify(salt_describe, get_salt());
        add_modify(device_id_describe, get_device_id());
        add_modify(app_id_describe, get_app_id());
        add_modify(create_time_describe, get_create_time());
        add_modify(status_describe, get_status());
    }
}

celeritas::account::account(const database_type database_type, traits::param_type::int64_type account_id)
    : base_type{ database_type, database_name, get_key_basis_database_container(database_type, account_id) },
      account_id_{ account_id },
      account_name_{ traits::string_type{} },
      password_hash_{ traits::string_type{} },
      salt_{ traits::string_type{} },
      device_id_{ traits::string_type{} },
      app_id_{ traits::int64_type{} },
      create_time_{ traits::int64_type{} },
      status_{ traits::int32_type{} }
{
    add_modify(account_id_describe, account_id);
}

celeritas::traits::int64_type celeritas::account::get_account_id() const noexcept
{
    return account_id_.get_value();
}

celeritas::traits::string_type celeritas::account::get_account_name() const
{
    return account_name_.get_value();
}

celeritas::traits::string_type celeritas::account::get_password_hash() const
{
    return password_hash_.get_value();
}

celeritas::traits::string_type celeritas::account::get_salt() const
{
    return salt_.get_value();
}

celeritas::traits::string_type celeritas::account::get_device_id() const
{
    return device_id_.get_value();
}

celeritas::traits::int64_type celeritas::account::get_app_id() const noexcept
{
    return app_id_.get_value();
}

celeritas::traits::int64_type celeritas::account::get_create_time() const noexcept
{
    return create_time_.get_value();
}

celeritas::traits::int32_type celeritas::account::get_status() const noexcept
{
    return status_.get_value();
}

void celeritas::account::set_account_id(traits::param_type::int64_type account_id)
{
    if (account_id != get_account_id())
    {
        account_id_.set_value(account_id);

        add_modify(account_id_describe, get_account_id());
    }
}

void celeritas::account::set_account_name(traits::param_type::string_type account_name)
{
    if (account_name != get_account_name())
    {
        account_name_.set_value(account_name);

        add_modify(account_name_describe, get_account_name());
    }
}

void celeritas::account::set_password_hash(traits::param_type::string_type password_hash)
{
    if (password_hash != get_password_hash())
    {
        password_hash_.set_value(password_hash);

        add_modify(password_hash_describe, get_password_hash());
    }
}

void celeritas::account::set_salt(traits::param_type::string_type salt)
{
    if (salt != get_salt())
    {
        salt_.set_value(salt);

        add_modify(salt_describe, get_salt());
    }
}

void celeritas::account::set_device_id(traits::param_type::string_type device_id)
{
    if (device_id != get_device_id())
    {
        device_id_.set_value(device_id);

        add_modify(device_id_describe, get_device_id());
    }
}

void celeritas::account::set_app_id(traits::param_type::int64_type app_id)
{
    if (app_id != get_app_id())
    {
        app_id_.set_value(app_id);

        add_modify(app_id_describe, get_app_id());
    }
}

void celeritas::account::set_create_time(traits::param_type::int64_type create_time)
{
    if (create_time != get_create_time())
    {
        create_time_.set_value(create_time);

        add_modify(create_time_describe, get_create_time());
    }
}

void celeritas::account::set_status(traits::param_type::int32_type status)
{
    if (status != get_status())
    {
        status_.set_value(status);

        add_modify(status_describe, get_status());
    }
}

const celeritas::database_entity::database_field_container& celeritas::account::get_database_field_container()
{
    static const database_field_container field_name_container{ decltype(account_id_)::get_database_field(),
                                                                decltype(account_name_)::get_database_field(),
                                                                decltype(password_hash_)::get_database_field(),
                                                                decltype(salt_)::get_database_field(),
                                                                decltype(device_id_)::get_database_field(),
                                                                decltype(app_id_)::get_database_field(),
                                                                decltype(create_time_)::get_database_field(),
                                                                decltype(status_)::get_database_field() };

    return field_name_container;
}

celeritas::account::database_entity_change_const_shared_ptr celeritas::account::get_select(const database_type database_type)
{
    static const auto result = std::make_shared<database_entity_change>(database_type,
                                                                        database_name,
                                                                        database_change_type::select_type);

    return result;
}

celeritas::account::database_entity_change_const_shared_ptr celeritas::account::get_select(const database_type database_type, traits::param_type::int64_type account_id)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    get_key_basis_database_container(database_type, account_id));
}

celeritas::account::database_entity_change_const_shared_ptr celeritas::account::get_select(const database_type database_type, const basis_database_container_const_shared_ptr& key)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    key);
}

celeritas::account::basis_database_container_const_shared_ptr celeritas::account::get_key_basis_database_container(const database_type database_type, traits::param_type::int64_type account_id)
{
    return std::make_shared<basis_database_container>(basis_database{ account_id_describe, account_id });
}