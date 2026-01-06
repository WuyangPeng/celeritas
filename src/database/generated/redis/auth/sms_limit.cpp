// 此文件是自动生成，请勿手动修改。

#include "sms_limit.h"
#include "config/basic/database_type.h"
#include "database/basic/database_change_type.h"
#include "database/basic/database_entity_change.tpp"
#include "database/entity/database_entity.tpp"
#include "database/entity/entity.tpp"

celeritas::sms_limit::sms_limit(const database_entity_change& entity)
    : base_type{ entity },
      phone_{ entity.get_value<database_data_type::string_type>(phone_describe) },
      exist_{ entity.get_value<database_data_type::bool_type>(exist_describe) }
{
}

celeritas::sms_limit::sms_limit(const database_type database_type, const database_entity_change& entity)
    : base_type{ database_type, entity },
      phone_{ entity.get_value<database_data_type::string_type>(phone_describe) },
      exist_{ entity.get_value<database_data_type::bool_type>(exist_describe) }
{
    if (database_type != entity.get_database_type())
    {
        add_modify(phone_describe, get_phone());
        add_modify(exist_describe, is_exist());
    }
}

celeritas::sms_limit::sms_limit(const database_type database_type, traits::param_type::string_type phone)
    : base_type{ database_type, database_name, get_key_basis_database_container(phone) },
      phone_{ phone },
      exist_{ traits::bool_type{} }
{
    add_modify(phone_describe, phone);
}

celeritas::traits::string_type celeritas::sms_limit::get_phone() const
{
    return phone_.get_value();
}

celeritas::traits::bool_type celeritas::sms_limit::is_exist() const noexcept
{
    return exist_.get_value();
}

void celeritas::sms_limit::set_phone(traits::param_type::string_type phone)
{
    if (phone != get_phone())
    {
        phone_.set_value(phone);

        add_modify(phone_describe, get_phone());
    }
}

void celeritas::sms_limit::set_exist(traits::param_type::bool_type exist)
{
    if (exist != is_exist())
    {
        exist_.set_value(exist);

        add_modify(exist_describe, is_exist());
    }
}

const celeritas::database_entity::database_field_container& celeritas::sms_limit::get_database_field_container()
{
    static const database_field_container field_name_container{ decltype(phone_)::get_database_field(),
                                                                decltype(exist_)::get_database_field() };

    return field_name_container;
}

celeritas::sms_limit::database_entity_change_const_shared_ptr celeritas::sms_limit::get_select(const database_type database_type)
{
    static const auto result = std::make_shared<database_entity_change>(database_type,
                                                                        database_name,
                                                                        database_change_type::select_type);

    return result;
}

celeritas::sms_limit::database_entity_change_const_shared_ptr celeritas::sms_limit::get_select(const database_type database_type, traits::param_type::string_type phone)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    get_key_basis_database_container(phone));
}

celeritas::sms_limit::database_entity_change_const_shared_ptr celeritas::sms_limit::get_select(const database_type database_type, const basis_database_container_const_shared_ptr& key)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    key);
}

celeritas::sms_limit::basis_database_container_const_shared_ptr celeritas::sms_limit::get_key_basis_database_container(traits::param_type::string_type phone)
{
    return std::make_shared<basis_database_container>(basis_database{ phone_describe, phone });
}