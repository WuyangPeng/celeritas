// 此文件是自动生成，请勿手动修改。

#include "user_red_dots.h"
#include "config/basic/database_type.h"
#include "database/basic/basis_database.tpp"
#include "database/basic/database_change_type.h"
#include "database/basic/database_entity_change.tpp"
#include "database/entity/database_entity.h"
#include "database/entity/entity.tpp"

celeritas::user_red_dots::user_red_dots(const database_entity_change& entity)
    : base_type{ entity },
      user_id_{ entity.get_value<database_data_type::int64_type>(user_id_describe) },
      red_dots_{ entity.get_value<database_data_type::document_array_type>(red_dots_describe) },
      last_check_time_{ entity.get_value<database_data_type::int64_type>(last_check_time_describe) }
{
}

celeritas::user_red_dots::user_red_dots(const database_type database_type, const database_entity_change& entity)
    : base_type{ database_type, entity },
      user_id_{ entity.get_value<database_data_type::int64_type>(user_id_describe) },
      red_dots_{ entity.get_value<database_data_type::document_array_type>(red_dots_describe) },
      last_check_time_{ entity.get_value<database_data_type::int64_type>(last_check_time_describe) }
{
    if (database_type != entity.get_database_type())
    {
        add_modify(user_id_describe, get_user_id());
        add_modify(red_dots_describe, get_red_dots());
        add_modify(last_check_time_describe, get_last_check_time());
    }
}

celeritas::user_red_dots::user_red_dots(const database_type database_type, traits::param_type::int64_type user_id)
    : base_type{ database_type, database_name, get_key_basis_database_container(user_id) },
      user_id_{ user_id },
      red_dots_{ traits::document_array_type{} },
      last_check_time_{ traits::int64_type{} }
{
    add_modify(user_id_describe, user_id);
}

celeritas::traits::int64_type celeritas::user_red_dots::get_user_id() const noexcept
{
    return user_id_.get_value();
}

celeritas::traits::document_array_type celeritas::user_red_dots::get_red_dots() const
{
    return red_dots_.get_value();
}

celeritas::traits::int64_type celeritas::user_red_dots::get_last_check_time() const noexcept
{
    return last_check_time_.get_value();
}

void celeritas::user_red_dots::set_user_id(traits::param_type::int64_type user_id)
{
    if (user_id != get_user_id())
    {
        user_id_.set_value(user_id);

        add_modify(user_id_describe, get_user_id());
    }
}

void celeritas::user_red_dots::set_red_dots(traits::param_type::document_array_type red_dots)
{
    if (red_dots != get_red_dots())
    {
        red_dots_.set_value(red_dots);

        add_modify(red_dots_describe, get_red_dots());
    }
}

void celeritas::user_red_dots::set_last_check_time(traits::param_type::int64_type last_check_time)
{
    if (last_check_time != get_last_check_time())
    {
        last_check_time_.set_value(last_check_time);

        add_modify(last_check_time_describe, get_last_check_time());
    }
}

void celeritas::user_red_dots::set_red_dots(const int index, traits::param_type::document_array_element_type red_dots)
{
    if (red_dots_.set_value(index, red_dots))
    {
        add_modify(red_dots_describe, get_red_dots());
    }
}

void celeritas::user_red_dots::add_red_dots(traits::param_type::document_array_element_type red_dots)
{
    red_dots_.add_value(red_dots);

    add_modify(red_dots_describe, get_red_dots());
}

void celeritas::user_red_dots::remove_red_dots(const int index)
{
    red_dots_.remove_value(index);

    add_modify(red_dots_describe, get_red_dots());
}

const celeritas::database_entity::database_field_container& celeritas::user_red_dots::get_database_field_container()
{
    static const database_field_container field_name_container{ decltype(user_id_)::get_database_field(),
                                                                decltype(red_dots_)::get_database_field(),
                                                                decltype(last_check_time_)::get_database_field() };

    return field_name_container;
}

celeritas::user_red_dots::const_database_entity_change_shared_ptr celeritas::user_red_dots::get_select(const database_type database_type)
{
    static const auto result = std::make_shared<database_entity_change>(database_type,
                                                                        database_name,
                                                                        database_change_type::select_type);

    return result;
}

celeritas::user_red_dots::const_database_entity_change_shared_ptr celeritas::user_red_dots::get_select(const database_type database_type, traits::param_type::int64_type user_id)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    get_key_basis_database_container(user_id));
}

celeritas::user_red_dots::const_database_entity_change_shared_ptr celeritas::user_red_dots::get_select(const database_type database_type, const const_basis_database_container_shared_ptr& key)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    key);
}

celeritas::user_red_dots::const_basis_database_container_shared_ptr celeritas::user_red_dots::get_key_basis_database_container(traits::param_type::int64_type user_id)
{
    return std::make_shared<basis_database_container>(basis_database{ user_id_describe, user_id });
}