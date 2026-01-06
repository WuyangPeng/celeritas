// 此文件是自动生成，请勿手动修改。

#include "time_refresh.h"
#include "config/basic/database_type.h"
#include "database/basic/database_change_type.h"
#include "database/basic/database_entity_change.tpp"
#include "database/entity/database_entity.tpp"
#include "database/entity/entity.tpp"

celeritas::time_refresh::time_refresh(const database_entity_change& entity)
    : base_type{ entity },
      id_{ entity.get_value<database_data_type::int64_type>(id_describe) },
      time_refresh_type_{ entity.get_value<database_data_type::int32_type>(time_refresh_type_describe) },
      parameter_{ entity.get_value<database_data_type::int32_type>(parameter_describe) }
{
}

celeritas::time_refresh::time_refresh(const database_type database_type, const database_entity_change& entity)
    : base_type{ database_type, entity },
      id_{ entity.get_value<database_data_type::int64_type>(id_describe) },
      time_refresh_type_{ entity.get_value<database_data_type::int32_type>(time_refresh_type_describe) },
      parameter_{ entity.get_value<database_data_type::int32_type>(parameter_describe) }
{
    if (database_type != entity.get_database_type())
    {
        add_modify(id_describe, get_id());
        add_modify(time_refresh_type_describe, get_time_refresh_type());
        add_modify(parameter_describe, get_parameter());
    }
}

celeritas::time_refresh::time_refresh(const database_type database_type, traits::param_type::int64_type id)
    : base_type{ database_type, database_name, get_key_basis_database_container(id) },
      id_{ id },
      time_refresh_type_{ traits::int32_type{} },
      parameter_{ traits::int32_type{} }
{
    add_modify(id_describe, id);
}

celeritas::traits::int64_type celeritas::time_refresh::get_id() const noexcept
{
    return id_.get_value();
}

celeritas::traits::int32_type celeritas::time_refresh::get_time_refresh_type() const noexcept
{
    return time_refresh_type_.get_value();
}

celeritas::traits::int32_type celeritas::time_refresh::get_parameter() const noexcept
{
    return parameter_.get_value();
}

void celeritas::time_refresh::set_id(traits::param_type::int64_type id)
{
    if (id != get_id())
    {
        id_.set_value(id);

        add_modify(id_describe, get_id());
    }
}

void celeritas::time_refresh::set_time_refresh_type(traits::param_type::int32_type time_refresh_type)
{
    if (time_refresh_type != get_time_refresh_type())
    {
        time_refresh_type_.set_value(time_refresh_type);

        add_modify(time_refresh_type_describe, get_time_refresh_type());
    }
}

void celeritas::time_refresh::set_parameter(traits::param_type::int32_type parameter)
{
    if (parameter != get_parameter())
    {
        parameter_.set_value(parameter);

        add_modify(parameter_describe, get_parameter());
    }
}

const celeritas::database_entity::database_field_container& celeritas::time_refresh::get_database_field_container()
{
    static const database_field_container field_name_container{ decltype(id_)::get_database_field(),
                                                                decltype(time_refresh_type_)::get_database_field(),
                                                                decltype(parameter_)::get_database_field() };

    return field_name_container;
}

celeritas::time_refresh::database_entity_change_const_shared_ptr celeritas::time_refresh::get_select(const database_type database_type)
{
    static const auto result = std::make_shared<database_entity_change>(database_type,
                                                                        database_name,
                                                                        database_change_type::select_type);

    return result;
}

celeritas::time_refresh::database_entity_change_const_shared_ptr celeritas::time_refresh::get_select(const database_type database_type, traits::param_type::int64_type id)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    get_key_basis_database_container(id));
}

celeritas::time_refresh::database_entity_change_const_shared_ptr celeritas::time_refresh::get_select(const database_type database_type, const basis_database_container_const_shared_ptr& key)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    key);
}

celeritas::time_refresh::basis_database_container_const_shared_ptr celeritas::time_refresh::get_key_basis_database_container(traits::param_type::int64_type id)
{
    return std::make_shared<basis_database_container>(basis_database{ id_describe, id });
}