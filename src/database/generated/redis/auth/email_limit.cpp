// 此文件是自动生成，请勿手动修改。

#include "email_limit.h"
#include "config/database_type.h"
#include "database/database_change_type.h"
#include "database/database_entity_change.tpp"
#include "database/database_entity.tpp"
#include "database/entity.tpp"

celeritas::email_limit::email_limit(const database_entity_change& entity)
    : base_type{ entity },
      email_{ entity.get_value<database_data_type::string_type>(email_describe) },
      exist_{ entity.get_value<database_data_type::bool_type>(exist_describe) }
{
}

celeritas::email_limit::email_limit(const database_type database_type, const database_entity_change& entity)
    : base_type{ database_type, entity },
      email_{ entity.get_value<database_data_type::string_type>(email_describe) },
      exist_{ entity.get_value<database_data_type::bool_type>(exist_describe) }
{
    if (database_type != entity.get_database_type())
    {
        add_modify(email_describe, get_email());
        add_modify(exist_describe, is_exist());
    }
}

celeritas::email_limit::email_limit(const database_type database_type, traits::param_type::string_type email)
    : base_type{ database_type, database_name, get_key_basis_database_container(email) },
      email_{ email },
      exist_{ traits::bool_type{} }
{
    add_modify(email_describe, email);
}

celeritas::traits::string_type celeritas::email_limit::get_email() const
{
    return email_.get_value();
}

celeritas::traits::bool_type celeritas::email_limit::is_exist() const noexcept
{
    return exist_.get_value();
}

void celeritas::email_limit::set_email(traits::param_type::string_type email)
{
    if (email != get_email())
    {
        email_.set_value(email);

        add_modify(email_describe, get_email());
    }
}

void celeritas::email_limit::set_exist(traits::param_type::bool_type exist)
{
    if (exist != is_exist())
    {
        exist_.set_value(exist);

        add_modify(exist_describe, is_exist());
    }
}

const celeritas::database_entity::database_field_container& celeritas::email_limit::get_database_field_container()
{
    static const database_field_container field_name_container{ decltype(email_)::get_database_field(),
                                                                decltype(exist_)::get_database_field() };

    return field_name_container;
}

celeritas::email_limit::database_entity_change_const_shared_ptr celeritas::email_limit::get_select(const database_type database_type)
{
    static const auto result = std::make_shared<database_entity_change>(database_type,
                                                                        database_name,
                                                                        database_change_type::select_type);

    return result;
}

celeritas::email_limit::database_entity_change_const_shared_ptr celeritas::email_limit::get_select(const database_type database_type, traits::param_type::string_type email)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    get_key_basis_database_container(email));
}

celeritas::email_limit::database_entity_change_const_shared_ptr celeritas::email_limit::get_select(const database_type database_type, const basis_database_container_const_shared_ptr& key)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    key);
}

celeritas::email_limit::basis_database_container_const_shared_ptr celeritas::email_limit::get_key_basis_database_container(traits::param_type::string_type email)
{
    return std::make_shared<basis_database_container>(basis_database{ email_describe, email });
}