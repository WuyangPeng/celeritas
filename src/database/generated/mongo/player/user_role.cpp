// 此文件是自动生成，请勿手动修改。

#include "user_role.h"
#include "config/basic/database_type.h"
#include "database/basic/basis_database.tpp"
#include "database/basic/database_change_type.h"
#include "database/basic/database_entity_change.tpp"
#include "database/entity/database_entity.h"
#include "database/entity/entity.tpp"

celeritas::user_role::user_role(const database_entity_change& entity)
    : base_type{ entity },
      user_id_{ entity.get_value<database_data_type::int64_type>(user_id_describe) },
      surname_{ entity.get_value<database_data_type::string_type>(surname_describe) },
      name_{ entity.get_value<database_data_type::string_type>(name_describe) },
      modify_name_{ entity.get_value<database_data_type::bool_type>(modify_name_describe) },
      change_count_{ entity.get_value<database_data_type::int32_type>(change_count_describe) },
      per_day_change_count_{ entity.get_value<database_data_type::int32_type>(per_day_change_count_describe) },
      device_id_{ entity.get_value<database_data_type::string_type>(device_id_describe) },
      app_version_{ entity.get_value<database_data_type::string_type>(app_version_describe) },
      change_name_time_{ entity.get_value<database_data_type::int64_type>(change_name_time_describe) },
      full_name_{ entity.get_value<database_data_type::string_type>(full_name_describe) }
{
}

celeritas::user_role::user_role(const database_type database_type, const database_entity_change& entity)
    : base_type{ database_type, entity },
      user_id_{ entity.get_value<database_data_type::int64_type>(user_id_describe) },
      surname_{ entity.get_value<database_data_type::string_type>(surname_describe) },
      name_{ entity.get_value<database_data_type::string_type>(name_describe) },
      modify_name_{ entity.get_value<database_data_type::bool_type>(modify_name_describe) },
      change_count_{ entity.get_value<database_data_type::int32_type>(change_count_describe) },
      per_day_change_count_{ entity.get_value<database_data_type::int32_type>(per_day_change_count_describe) },
      device_id_{ entity.get_value<database_data_type::string_type>(device_id_describe) },
      app_version_{ entity.get_value<database_data_type::string_type>(app_version_describe) },
      change_name_time_{ entity.get_value<database_data_type::int64_type>(change_name_time_describe) },
      full_name_{ entity.get_value<database_data_type::string_type>(full_name_describe) }
{
    if (database_type != entity.get_database_type())
    {
        add_modify(user_id_describe, get_user_id());
        add_modify(surname_describe, get_surname());
        add_modify(name_describe, get_name());
        add_modify(modify_name_describe, is_modify_name());
        add_modify(change_count_describe, get_change_count());
        add_modify(per_day_change_count_describe, get_per_day_change_count());
        add_modify(device_id_describe, get_device_id());
        add_modify(app_version_describe, get_app_version());
        add_modify(change_name_time_describe, get_change_name_time());
        add_modify(full_name_describe, get_full_name());
    }
}

celeritas::user_role::user_role(const database_type database_type, traits::param_type::int64_type user_id)
    : base_type{ database_type, database_name, get_key_basis_database_container(user_id) },
      user_id_{ user_id },
      surname_{ traits::string_type{} },
      name_{ traits::string_type{} },
      modify_name_{ traits::bool_type{} },
      change_count_{ traits::int32_type{} },
      per_day_change_count_{ traits::int32_type{} },
      device_id_{ traits::string_type{} },
      app_version_{ traits::string_type{} },
      change_name_time_{ traits::int64_type{} },
      full_name_{ traits::string_type{} }
{
    add_modify(user_id_describe, user_id);
}

celeritas::traits::int64_type celeritas::user_role::get_user_id() const noexcept
{
    return user_id_.get_value();
}

celeritas::traits::string_type celeritas::user_role::get_surname() const
{
    return surname_.get_value();
}

celeritas::traits::string_type celeritas::user_role::get_name() const
{
    return name_.get_value();
}

celeritas::traits::bool_type celeritas::user_role::is_modify_name() const noexcept
{
    return modify_name_.get_value();
}

celeritas::traits::int32_type celeritas::user_role::get_change_count() const noexcept
{
    return change_count_.get_value();
}

celeritas::traits::int32_type celeritas::user_role::get_per_day_change_count() const noexcept
{
    return per_day_change_count_.get_value();
}

celeritas::traits::string_type celeritas::user_role::get_device_id() const
{
    return device_id_.get_value();
}

celeritas::traits::string_type celeritas::user_role::get_app_version() const
{
    return app_version_.get_value();
}

celeritas::traits::int64_type celeritas::user_role::get_change_name_time() const noexcept
{
    return change_name_time_.get_value();
}

celeritas::traits::string_type celeritas::user_role::get_full_name() const
{
    return full_name_.get_value();
}

void celeritas::user_role::set_user_id(traits::param_type::int64_type user_id)
{
    if (user_id != get_user_id())
    {
        user_id_.set_value(user_id);

        add_modify(user_id_describe, get_user_id());
    }
}

void celeritas::user_role::set_surname(traits::param_type::string_type surname)
{
    if (surname != get_surname())
    {
        surname_.set_value(surname);

        add_modify(surname_describe, get_surname());
    }
}

void celeritas::user_role::set_name(traits::param_type::string_type name)
{
    if (name != get_name())
    {
        name_.set_value(name);

        add_modify(name_describe, get_name());
    }
}

void celeritas::user_role::set_modify_name(traits::param_type::bool_type modify_name)
{
    if (modify_name != is_modify_name())
    {
        modify_name_.set_value(modify_name);

        add_modify(modify_name_describe, is_modify_name());
    }
}

void celeritas::user_role::set_change_count(traits::param_type::int32_type change_count)
{
    if (change_count != get_change_count())
    {
        change_count_.set_value(change_count);

        add_modify(change_count_describe, get_change_count());
    }
}

void celeritas::user_role::set_per_day_change_count(traits::param_type::int32_type per_day_change_count)
{
    if (per_day_change_count != get_per_day_change_count())
    {
        per_day_change_count_.set_value(per_day_change_count);

        add_modify(per_day_change_count_describe, get_per_day_change_count());
    }
}

void celeritas::user_role::set_device_id(traits::param_type::string_type device_id)
{
    if (device_id != get_device_id())
    {
        device_id_.set_value(device_id);

        add_modify(device_id_describe, get_device_id());
    }
}

void celeritas::user_role::set_app_version(traits::param_type::string_type app_version)
{
    if (app_version != get_app_version())
    {
        app_version_.set_value(app_version);

        add_modify(app_version_describe, get_app_version());
    }
}

void celeritas::user_role::set_change_name_time(traits::param_type::int64_type change_name_time)
{
    if (change_name_time != get_change_name_time())
    {
        change_name_time_.set_value(change_name_time);

        add_modify(change_name_time_describe, get_change_name_time());
    }
}

void celeritas::user_role::set_full_name(traits::param_type::string_type full_name)
{
    if (full_name != get_full_name())
    {
        full_name_.set_value(full_name);

        add_modify(full_name_describe, get_full_name());
    }
}

const celeritas::database_entity::database_field_container& celeritas::user_role::get_database_field_container()
{
    static const database_field_container field_name_container{ decltype(user_id_)::get_database_field(),
                                                                decltype(surname_)::get_database_field(),
                                                                decltype(name_)::get_database_field(),
                                                                decltype(modify_name_)::get_database_field(),
                                                                decltype(change_count_)::get_database_field(),
                                                                decltype(per_day_change_count_)::get_database_field(),
                                                                decltype(device_id_)::get_database_field(),
                                                                decltype(app_version_)::get_database_field(),
                                                                decltype(change_name_time_)::get_database_field(),
                                                                decltype(full_name_)::get_database_field() };

    return field_name_container;
}

celeritas::user_role::const_database_entity_change_shared_ptr celeritas::user_role::get_select(const database_type database_type)
{
    static const auto result = std::make_shared<database_entity_change>(database_type,
                                                                        database_name,
                                                                        database_change_type::select_type);

    return result;
}

celeritas::user_role::const_database_entity_change_shared_ptr celeritas::user_role::get_select(const database_type database_type, traits::param_type::int64_type user_id)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    get_key_basis_database_container(user_id));
}

celeritas::user_role::const_database_entity_change_shared_ptr celeritas::user_role::get_select(const database_type database_type, const const_basis_database_container_shared_ptr& key)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    key);
}

celeritas::user_role::const_basis_database_container_shared_ptr celeritas::user_role::get_key_basis_database_container(traits::param_type::int64_type user_id)
{
    return std::make_shared<basis_database_container>(basis_database{ user_id_describe, user_id });
}