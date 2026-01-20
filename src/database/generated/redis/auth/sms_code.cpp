// 此文件是自动生成，请勿手动修改。

#include "sms_code.h"
#include "config/basic/database_type.h"
#include "database/basic/basis_database.tpp"
#include "database/basic/database_change_type.h"
#include "database/basic/database_entity_change.tpp"
#include "database/entity/database_entity.h"
#include "database/entity/entity.tpp"

celeritas::sms_code::sms_code(const database_entity_change& entity)
    : base_type{ entity.get_database_entity_change(phone_describe)  },
      phone_{ entity.get_value<database_data_type::string_type>(phone_describe) },
      code_{ entity.get_value<database_data_type::int32_type>(code_describe) },
      retry_count_{ entity.get_value<database_data_type::int32_count_type>(retry_count_describe) }
{
}

celeritas::sms_code::sms_code(const database_type database_type, const database_entity_change& entity)
    : base_type{ database_type, entity.get_database_entity_change(phone_describe) },
      phone_{ entity.get_value<database_data_type::string_type>(phone_describe) },
      code_{ entity.get_value<database_data_type::int32_type>(code_describe) },
      retry_count_{ entity.get_value<database_data_type::int32_count_type>(retry_count_describe) }
{
    if (database_type != entity.get_database_type())
    {
        add_modify(phone_describe, get_phone());
        add_modify(code_describe, get_code());
        add_modify(retry_count_describe, get_retry_count());
    }
}

celeritas::sms_code::sms_code(const database_type database_type, traits::param_type::string_type phone)
    : base_type{ database_type, database_name, get_key_basis_database_container(phone) },
      phone_{ phone },
      code_{ traits::int32_type{} },
      retry_count_{ traits::int32_count_type{} }
{
    add_modify(phone_describe, phone);
}

celeritas::traits::string_type celeritas::sms_code::get_phone() const
{
    return phone_.get_value();
}

celeritas::traits::int32_type celeritas::sms_code::get_code() const noexcept
{
    return code_.get_value();
}

celeritas::traits::int32_count_type celeritas::sms_code::get_retry_count() const noexcept
{
    return retry_count_.get_value();
}

void celeritas::sms_code::set_phone(traits::param_type::string_type phone)
{
    if (phone != get_phone())
    {
        phone_.set_value(phone);

        add_modify(phone_describe, get_phone());
    }
}

void celeritas::sms_code::set_code(traits::param_type::int32_type code)
{
    if (code != get_code())
    {
        code_.set_value(code);

        add_modify(code_describe, get_code());
    }
}

void celeritas::sms_code::set_retry_count(traits::param_type::int32_count_type retry_count)
{
    if (retry_count != get_retry_count())
    {
        retry_count_.set_value(retry_count);

        add_modify(retry_count_describe, get_retry_count());
    }
}

void celeritas::sms_code::modify_retry_count(traits::param_type::int32_count_type retry_count)
{
    retry_count_.modify_value(retry_count);

    add_modify(retry_count_describe, get_retry_count());
}

const celeritas::database_entity::database_field_container& celeritas::sms_code::get_database_field_container()
{
    static const database_field_container field_name_container{ decltype(phone_)::get_database_field(),
                                                                decltype(code_)::get_database_field(),
                                                                decltype(retry_count_)::get_database_field() };

    return field_name_container;
}

celeritas::sms_code::const_database_entity_change_shared_ptr celeritas::sms_code::get_select(const database_type database_type)
{
    static const auto result = std::make_shared<database_entity_change>(database_type,
                                                                        database_name,
                                                                        database_change_type::select_type);

    return result;
}

celeritas::sms_code::const_database_entity_change_shared_ptr celeritas::sms_code::get_select(const database_type database_type, traits::param_type::string_type phone)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    get_key_basis_database_container(phone));
}

celeritas::sms_code::const_database_entity_change_shared_ptr celeritas::sms_code::get_select(const database_type database_type, const const_basis_database_container_shared_ptr& key)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    key);
}

celeritas::sms_code::const_basis_database_container_shared_ptr celeritas::sms_code::get_key_basis_database_container(traits::param_type::string_type phone)
{
    return std::make_shared<basis_database_container>(basis_database{ phone_describe, phone });
}