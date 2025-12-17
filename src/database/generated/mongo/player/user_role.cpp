// 此文件是自动生成，请勿手动修改。

#include "user_role.h"
#include "config/database_type.h"
#include "database/database_change_type.h"
#include "database/database_entity_change.tpp"
#include "database/database_entity.tpp"
#include "database/entity.tpp"

celeritas::user_role::user_role(const database_entity_change& entity)
    : base_type{ entity },
      user_id_{ entity.get_value<database_data_type::int64_type>(entity.get_database_type() == database_type::mongo ? "_id" : user_id_describe) },
      name_{ entity.get_value<database_data_type::string_type>(name_describe) },
      device_id_{ entity.get_value<database_data_type::string_type>(device_id_describe) },
      app_version_{ entity.get_value<database_data_type::string_type>(app_version_describe) }
{
}

celeritas::user_role::user_role(const database_type database_type, const database_entity_change& entity)
    : base_type{ database_type, entity },
      user_id_{ entity.get_value<database_data_type::int64_type>(entity.get_database_type() == database_type::mongo ? "_id" : user_id_describe) },
      name_{ entity.get_value<database_data_type::string_type>(name_describe) },
      device_id_{ entity.get_value<database_data_type::string_type>(device_id_describe) },
      app_version_{ entity.get_value<database_data_type::string_type>(app_version_describe) }
{
    if (database_type != entity.get_database_type())
    {
        add_modify(user_id_describe, get_user_id());
        add_modify(name_describe, get_name());
        add_modify(device_id_describe, get_device_id());
        add_modify(app_version_describe, get_app_version());
    }
}

celeritas::user_role::user_role(const database_type database_type, traits::param_type::int64_type user_id)
    : base_type{ database_type, database_name, get_key_basis_database_container(database_type, user_id) },
      user_id_{ user_id },
      name_{ traits::string_type{} },
      device_id_{ traits::string_type{} },
      app_version_{ traits::string_type{} }
{
    add_modify(user_id_describe, user_id);
}

celeritas::traits::int64_type celeritas::user_role::get_user_id() const noexcept
{
    return user_id_.get_value();
}

celeritas::traits::string_type celeritas::user_role::get_name() const
{
    return name_.get_value();
}

celeritas::traits::string_type celeritas::user_role::get_device_id() const
{
    return device_id_.get_value();
}

celeritas::traits::string_type celeritas::user_role::get_app_version() const
{
    return app_version_.get_value();
}

void celeritas::user_role::set_user_id(traits::param_type::int64_type user_id)
{
    if (user_id != get_user_id())
    {
        user_id_.set_value(user_id);

        add_modify(user_id_describe, get_user_id());
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

const celeritas::database_entity::database_field_container& celeritas::user_role::get_database_field_container()
{
    static const database_field_container field_name_container{ decltype(user_id_)::get_database_field(),
                                                                decltype(name_)::get_database_field(),
                                                                decltype(device_id_)::get_database_field(),
                                                                decltype(app_version_)::get_database_field() };

    return field_name_container;
}

celeritas::user_role::database_entity_change_const_shared_ptr celeritas::user_role::get_select(const database_type database_type)
{
    static const auto result = std::make_shared<database_entity_change>(database_type,
                                                                        database_name,
                                                                        database_change_type::select_type);

    return result;
}

celeritas::user_role::database_entity_change_const_shared_ptr celeritas::user_role::get_select(const database_type database_type, traits::param_type::int64_type user_id)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    get_key_basis_database_container(database_type, user_id));
}

celeritas::user_role::database_entity_change_const_shared_ptr celeritas::user_role::get_select(const database_type database_type, const basis_database_container_const_shared_ptr& key)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    key);
}

celeritas::user_role::basis_database_container_const_shared_ptr celeritas::user_role::get_key_basis_database_container(const database_type database_type, traits::param_type::int64_type user_id)
{
    const auto field_name = database_type == database_type::mongo ? "_id" : user_id_describe;

    const auto container = std::make_shared<basis_database_container>(basis_database{ field_name, user_id });

    return container;
}