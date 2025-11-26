// 此文件是自动生成，请勿手动修改。

#include "email_limit.h"
#include "config/database_type.h"
#include "database/basis_database_manager.tpp"
#include "database/database_change_type.h"
#include "database/database_entity.tpp"
#include "database/entity.tpp"

celeritas::email_limit celeritas::email_limit::create(const basis_database_manager& entity, const database_type database_type, traits::param_type::string_type email)
{
    return entity.is_modify() ? email_limit{ entity } : email_limit{ database_type, email };
}

celeritas::email_limit::email_limit(const basis_database_manager& entity)
    : base_type{ entity },
      email_{ entity.get_value<database_data_type::string_type>(entity.get_database_type() == database_type::mongo ? "_id" : email_describe) },
      exist_{ entity.get_value<database_data_type::bool_type>(exist_describe) }
{
}

celeritas::email_limit::email_limit(const database_type database_type, traits::param_type::string_type email)
    : base_type{ database_type, database_name.data(), get_key_basis_database_container(database_type, email) },
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

celeritas::email_limit::basis_database_manager_const_hared_ptr celeritas::email_limit::get_select(const database_type database_type, traits::param_type::string_type email)
{
    return std::make_shared<basis_database_manager>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    get_key_basis_database_container(database_type, email));
}

celeritas::email_limit::basis_database_manager_shared_ptr celeritas::email_limit::get_select(const database_type database_type)
{
    static const auto result = std::make_shared<basis_database_manager>(database_type,
                                                                        database_name,
                                                                        database_change_type::select_type,
                                                                        basis_database_container{});

    return result;
}

celeritas::basis_database_container celeritas::email_limit::get_key_basis_database_container(const database_type database_type, traits::param_type::string_type email)
{
    const auto field_name = database_type == database_type::mongo ? "_id" : email_describe;

    basis_database_container basis_database_container{ basis_database_container::object_container{ basis_database{ field_name, email } } };

    return basis_database_container;
}