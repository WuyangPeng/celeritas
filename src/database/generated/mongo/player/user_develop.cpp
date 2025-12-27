// 此文件是自动生成，请勿手动修改。

#include "user_develop.h"
#include "config/database_type.h"
#include "database/database_change_type.h"
#include "database/database_entity_change.tpp"
#include "database/database_entity.tpp"
#include "database/entity.tpp"

celeritas::user_develop::user_develop(const database_entity_change& entity)
    : base_type{ entity },
      user_id_{ entity.get_value<database_data_type::int64_type>(user_id_describe) },
      develop_data_{ entity.get_value<database_data_type::document_array_type>(develop_data_describe) },
      data_version_{ entity.get_value<database_data_type::int32_type>(data_version_describe) },
      last_save_time_{ entity.get_value<database_data_type::int64_type>(last_save_time_describe) }
{
}

celeritas::user_develop::user_develop(const database_type database_type, const database_entity_change& entity)
    : base_type{ database_type, entity },
      user_id_{ entity.get_value<database_data_type::int64_type>(user_id_describe) },
      develop_data_{ entity.get_value<database_data_type::document_array_type>(develop_data_describe) },
      data_version_{ entity.get_value<database_data_type::int32_type>(data_version_describe) },
      last_save_time_{ entity.get_value<database_data_type::int64_type>(last_save_time_describe) }
{
    if (database_type != entity.get_database_type())
    {
        add_modify(user_id_describe, get_user_id());
        add_modify(develop_data_describe, get_develop_data());
        add_modify(data_version_describe, get_data_version());
        add_modify(last_save_time_describe, get_last_save_time());
    }
}

celeritas::user_develop::user_develop(const database_type database_type, traits::param_type::int64_type user_id)
    : base_type{ database_type, database_name, get_key_basis_database_container(database_type, user_id) },
      user_id_{ user_id },
      develop_data_{ traits::document_array_type{} },
      data_version_{ traits::int32_type{} },
      last_save_time_{ traits::int64_type{} }
{
    add_modify(user_id_describe, user_id);
}

celeritas::traits::int64_type celeritas::user_develop::get_user_id() const noexcept
{
    return user_id_.get_value();
}

celeritas::traits::document_array_type celeritas::user_develop::get_develop_data() const
{
    return develop_data_.get_value();
}

celeritas::traits::int32_type celeritas::user_develop::get_data_version() const noexcept
{
    return data_version_.get_value();
}

celeritas::traits::int64_type celeritas::user_develop::get_last_save_time() const noexcept
{
    return last_save_time_.get_value();
}

void celeritas::user_develop::set_user_id(traits::param_type::int64_type user_id)
{
    if (user_id != get_user_id())
    {
        user_id_.set_value(user_id);

        add_modify(user_id_describe, get_user_id());
    }
}

void celeritas::user_develop::set_develop_data(traits::param_type::document_array_type develop_data)
{
    if (develop_data != get_develop_data())
    {
        develop_data_.set_value(develop_data);

        add_modify(develop_data_describe, get_develop_data());
    }
}

void celeritas::user_develop::set_data_version(traits::param_type::int32_type data_version)
{
    if (data_version != get_data_version())
    {
        data_version_.set_value(data_version);

        add_modify(data_version_describe, get_data_version());
    }
}

void celeritas::user_develop::set_last_save_time(traits::param_type::int64_type last_save_time)
{
    if (last_save_time != get_last_save_time())
    {
        last_save_time_.set_value(last_save_time);

        add_modify(last_save_time_describe, get_last_save_time());
    }
}

void celeritas::user_develop::set_develop_data(const int index, traits::param_type::document_array_element_type develop_data)
{
    if (develop_data_.set_value(index, develop_data))
    {
        add_modify(develop_data_describe, get_develop_data());
    }
}

void celeritas::user_develop::add_develop_data(traits::param_type::document_array_element_type develop_data)
{
    develop_data_.add_value(develop_data);

    add_modify(develop_data_describe, get_develop_data());
}

void celeritas::user_develop::remove_develop_data(const int index)
{
    develop_data_.remove_value(index);

    add_modify(develop_data_describe, get_develop_data());
}

const celeritas::database_entity::database_field_container& celeritas::user_develop::get_database_field_container()
{
    static const database_field_container field_name_container{ decltype(user_id_)::get_database_field(),
                                                                decltype(develop_data_)::get_database_field(),
                                                                decltype(data_version_)::get_database_field(),
                                                                decltype(last_save_time_)::get_database_field() };

    return field_name_container;
}

celeritas::user_develop::database_entity_change_const_shared_ptr celeritas::user_develop::get_select(const database_type database_type)
{
    static const auto result = std::make_shared<database_entity_change>(database_type,
                                                                        database_name,
                                                                        database_change_type::select_type);

    return result;
}

celeritas::user_develop::database_entity_change_const_shared_ptr celeritas::user_develop::get_select(const database_type database_type, traits::param_type::int64_type user_id)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    get_key_basis_database_container(database_type, user_id));
}

celeritas::user_develop::database_entity_change_const_shared_ptr celeritas::user_develop::get_select(const database_type database_type, const basis_database_container_const_shared_ptr& key)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    key);
}

celeritas::user_develop::basis_database_container_const_shared_ptr celeritas::user_develop::get_key_basis_database_container(const database_type database_type, traits::param_type::int64_type user_id)
{
    return std::make_shared<basis_database_container>(basis_database{ user_id_describe, user_id });
}