// 此文件是自动生成，请勿手动修改。

#include "admin_account.h"
#include "config/basic/database_type.h"
#include "database/basic/basis_database.tpp"
#include "database/basic/database_change_type.h"
#include "database/basic/database_entity_change.tpp"
#include "database/entity/database_entity.h"
#include "database/entity/entity.tpp"

celeritas::admin_account::admin_account(const database_entity_change& entity)
    : base_type{ entity.get_database_entity_change(account_id_describe)  },
      account_id_{ entity.get_value<database_data_type::int64_type>(account_id_describe) },
      username_{ entity.get_value<database_data_type::string_type>(username_describe) },
      app_id_{ entity.get_value<database_data_type::int64_type>(app_id_describe) },
      password_hash_{ entity.get_value<database_data_type::string_type>(password_hash_describe) },
      salt_{ entity.get_value<database_data_type::string_type>(salt_describe) },
      role_{ entity.get_value<database_data_type::int32_type>(role_describe) },
      nickname_{ entity.get_value<database_data_type::string_type>(nickname_describe) },
      avatar_{ entity.get_value<database_data_type::string_type>(avatar_describe) },
      creator_account_id_{ entity.get_value<database_data_type::int64_type>(creator_account_id_describe) },
      status_{ entity.get_value<database_data_type::int32_type>(status_describe) },
      create_time_{ entity.get_value<database_data_type::int64_type>(create_time_describe) },
      update_time_{ entity.get_value<database_data_type::int64_type>(update_time_describe) },
      last_login_time_{ entity.get_value<database_data_type::int64_type>(last_login_time_describe) },
      last_login_ip_{ entity.get_value<database_data_type::string_type>(last_login_ip_describe) }
{
}

celeritas::admin_account::admin_account(const database_type database_type, const database_entity_change& entity)
    : base_type{ database_type, entity.get_database_entity_change(account_id_describe) },
      account_id_{ entity.get_value<database_data_type::int64_type>(account_id_describe) },
      username_{ entity.get_value<database_data_type::string_type>(username_describe) },
      app_id_{ entity.get_value<database_data_type::int64_type>(app_id_describe) },
      password_hash_{ entity.get_value<database_data_type::string_type>(password_hash_describe) },
      salt_{ entity.get_value<database_data_type::string_type>(salt_describe) },
      role_{ entity.get_value<database_data_type::int32_type>(role_describe) },
      nickname_{ entity.get_value<database_data_type::string_type>(nickname_describe) },
      avatar_{ entity.get_value<database_data_type::string_type>(avatar_describe) },
      creator_account_id_{ entity.get_value<database_data_type::int64_type>(creator_account_id_describe) },
      status_{ entity.get_value<database_data_type::int32_type>(status_describe) },
      create_time_{ entity.get_value<database_data_type::int64_type>(create_time_describe) },
      update_time_{ entity.get_value<database_data_type::int64_type>(update_time_describe) },
      last_login_time_{ entity.get_value<database_data_type::int64_type>(last_login_time_describe) },
      last_login_ip_{ entity.get_value<database_data_type::string_type>(last_login_ip_describe) }
{
    if (database_type != entity.get_database_type())
    {
        add_modify(account_id_describe, get_account_id());
        add_modify(username_describe, get_username());
        add_modify(app_id_describe, get_app_id());
        add_modify(password_hash_describe, get_password_hash());
        add_modify(salt_describe, get_salt());
        add_modify(role_describe, get_role());
        add_modify(nickname_describe, get_nickname());
        add_modify(avatar_describe, get_avatar());
        add_modify(creator_account_id_describe, get_creator_account_id());
        add_modify(status_describe, get_status());
        add_modify(create_time_describe, get_create_time());
        add_modify(update_time_describe, get_update_time());
        add_modify(last_login_time_describe, get_last_login_time());
        add_modify(last_login_ip_describe, get_last_login_ip());
    }
}

celeritas::admin_account::admin_account(const database_type database_type, traits::param_type::int64_type account_id)
    : base_type{ database_type, database_name, get_key_basis_database_container(account_id) },
      account_id_{ account_id },
      username_{ traits::string_type{} },
      app_id_{ traits::int64_type{} },
      password_hash_{ traits::string_type{} },
      salt_{ traits::string_type{} },
      role_{ traits::int32_type{} },
      nickname_{ traits::string_type{} },
      avatar_{ traits::string_type{} },
      creator_account_id_{ traits::int64_type{} },
      status_{ traits::int32_type{} },
      create_time_{ traits::int64_type{} },
      update_time_{ traits::int64_type{} },
      last_login_time_{ traits::int64_type{} },
      last_login_ip_{ traits::string_type{} }
{
    add_modify(account_id_describe, account_id);
}

celeritas::traits::int64_type celeritas::admin_account::get_account_id() const noexcept
{
    return account_id_.get_value();
}

celeritas::traits::string_type celeritas::admin_account::get_username() const
{
    return username_.get_value();
}

celeritas::traits::int64_type celeritas::admin_account::get_app_id() const noexcept
{
    return app_id_.get_value();
}

celeritas::traits::string_type celeritas::admin_account::get_password_hash() const
{
    return password_hash_.get_value();
}

celeritas::traits::string_type celeritas::admin_account::get_salt() const
{
    return salt_.get_value();
}

celeritas::traits::int32_type celeritas::admin_account::get_role() const noexcept
{
    return role_.get_value();
}

celeritas::traits::string_type celeritas::admin_account::get_nickname() const
{
    return nickname_.get_value();
}

celeritas::traits::string_type celeritas::admin_account::get_avatar() const
{
    return avatar_.get_value();
}

celeritas::traits::int64_type celeritas::admin_account::get_creator_account_id() const noexcept
{
    return creator_account_id_.get_value();
}

celeritas::traits::int32_type celeritas::admin_account::get_status() const noexcept
{
    return status_.get_value();
}

celeritas::traits::int64_type celeritas::admin_account::get_create_time() const noexcept
{
    return create_time_.get_value();
}

celeritas::traits::int64_type celeritas::admin_account::get_update_time() const noexcept
{
    return update_time_.get_value();
}

celeritas::traits::int64_type celeritas::admin_account::get_last_login_time() const noexcept
{
    return last_login_time_.get_value();
}

celeritas::traits::string_type celeritas::admin_account::get_last_login_ip() const
{
    return last_login_ip_.get_value();
}

void celeritas::admin_account::set_account_id(traits::param_type::int64_type account_id)
{
    if (account_id != get_account_id())
    {
        account_id_.set_value(account_id);

        add_modify(account_id_describe, get_account_id());
    }
}

void celeritas::admin_account::set_username(traits::param_type::string_type username)
{
    if (username != get_username())
    {
        username_.set_value(username);

        add_modify(username_describe, get_username());
    }
}

void celeritas::admin_account::set_app_id(traits::param_type::int64_type app_id)
{
    if (app_id != get_app_id())
    {
        app_id_.set_value(app_id);

        add_modify(app_id_describe, get_app_id());
    }
}

void celeritas::admin_account::set_password_hash(traits::param_type::string_type password_hash)
{
    if (password_hash != get_password_hash())
    {
        password_hash_.set_value(password_hash);

        add_modify(password_hash_describe, get_password_hash());
    }
}

void celeritas::admin_account::set_salt(traits::param_type::string_type salt)
{
    if (salt != get_salt())
    {
        salt_.set_value(salt);

        add_modify(salt_describe, get_salt());
    }
}

void celeritas::admin_account::set_role(traits::param_type::int32_type role)
{
    if (role != get_role())
    {
        role_.set_value(role);

        add_modify(role_describe, get_role());
    }
}

void celeritas::admin_account::set_nickname(traits::param_type::string_type nickname)
{
    if (nickname != get_nickname())
    {
        nickname_.set_value(nickname);

        add_modify(nickname_describe, get_nickname());
    }
}

void celeritas::admin_account::set_avatar(traits::param_type::string_type avatar)
{
    if (avatar != get_avatar())
    {
        avatar_.set_value(avatar);

        add_modify(avatar_describe, get_avatar());
    }
}

void celeritas::admin_account::set_creator_account_id(traits::param_type::int64_type creator_account_id)
{
    if (creator_account_id != get_creator_account_id())
    {
        creator_account_id_.set_value(creator_account_id);

        add_modify(creator_account_id_describe, get_creator_account_id());
    }
}

void celeritas::admin_account::set_status(traits::param_type::int32_type status)
{
    if (status != get_status())
    {
        status_.set_value(status);

        add_modify(status_describe, get_status());
    }
}

void celeritas::admin_account::set_create_time(traits::param_type::int64_type create_time)
{
    if (create_time != get_create_time())
    {
        create_time_.set_value(create_time);

        add_modify(create_time_describe, get_create_time());
    }
}

void celeritas::admin_account::set_update_time(traits::param_type::int64_type update_time)
{
    if (update_time != get_update_time())
    {
        update_time_.set_value(update_time);

        add_modify(update_time_describe, get_update_time());
    }
}

void celeritas::admin_account::set_last_login_time(traits::param_type::int64_type last_login_time)
{
    if (last_login_time != get_last_login_time())
    {
        last_login_time_.set_value(last_login_time);

        add_modify(last_login_time_describe, get_last_login_time());
    }
}

void celeritas::admin_account::set_last_login_ip(traits::param_type::string_type last_login_ip)
{
    if (last_login_ip != get_last_login_ip())
    {
        last_login_ip_.set_value(last_login_ip);

        add_modify(last_login_ip_describe, get_last_login_ip());
    }
}

const celeritas::database_entity::database_field_container& celeritas::admin_account::get_database_field_container()
{
    static const database_field_container field_name_container{ decltype(account_id_)::get_database_field(),
                                                                decltype(username_)::get_database_field(),
                                                                decltype(app_id_)::get_database_field(),
                                                                decltype(password_hash_)::get_database_field(),
                                                                decltype(salt_)::get_database_field(),
                                                                decltype(role_)::get_database_field(),
                                                                decltype(nickname_)::get_database_field(),
                                                                decltype(avatar_)::get_database_field(),
                                                                decltype(creator_account_id_)::get_database_field(),
                                                                decltype(status_)::get_database_field(),
                                                                decltype(create_time_)::get_database_field(),
                                                                decltype(update_time_)::get_database_field(),
                                                                decltype(last_login_time_)::get_database_field(),
                                                                decltype(last_login_ip_)::get_database_field() };

    return field_name_container;
}

celeritas::admin_account::const_database_entity_change_shared_ptr celeritas::admin_account::get_select(const database_type database_type)
{
    static const auto result = std::make_shared<database_entity_change>(database_type,
                                                                        database_name,
                                                                        database_change_type::select_type);

    return result;
}

celeritas::admin_account::const_database_entity_change_shared_ptr celeritas::admin_account::get_select(const database_type database_type, traits::param_type::int64_type account_id)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    get_key_basis_database_container(account_id));
}

celeritas::admin_account::const_database_entity_change_shared_ptr celeritas::admin_account::get_select(const database_type database_type, const const_basis_database_container_shared_ptr& key)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    key);
}

celeritas::admin_account::const_basis_database_container_shared_ptr celeritas::admin_account::get_key_basis_database_container(traits::param_type::int64_type account_id)
{
    return std::make_shared<basis_database_container>(basis_database{ account_id_describe, account_id });
}