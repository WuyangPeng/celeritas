// 此文件是自动生成，请勿手动修改。

#include "email_code.h"
#include "config/database_type.h"
#include "database/database_change_type.h"
#include "database/database_entity_change.tpp"
#include "database/database_entity.tpp"
#include "database/entity.tpp"

celeritas::email_code::email_code(const database_entity_change& entity)
    : base_type{ entity },
      email_{ entity.get_value<database_data_type::string_type>(entity.get_database_type() == database_type::mongo ? "_id" : email_describe) },
      code_{ entity.get_value<database_data_type::int32_type>(code_describe) },
      retry_count_{ entity.get_value<database_data_type::int32_count_type>(retry_count_describe) }
{
}

celeritas::email_code::email_code(const database_type database_type, const database_entity_change& entity)
    : base_type{ database_type, entity },
      email_{ entity.get_value<database_data_type::string_type>(entity.get_database_type() == database_type::mongo ? "_id" : email_describe) },
      code_{ entity.get_value<database_data_type::int32_type>(code_describe) },
      retry_count_{ entity.get_value<database_data_type::int32_count_type>(retry_count_describe) }
{
    if (database_type != entity.get_database_type())
    {
        add_modify(email_describe, get_email());
        add_modify(code_describe, get_code());
        add_modify(retry_count_describe, get_retry_count());
    }
}

celeritas::email_code::email_code(const database_type database_type, traits::param_type::string_type email)
    : base_type{ database_type, database_name, get_key_basis_database_container(database_type, email) },
      email_{ email },
      code_{ traits::int32_type{} },
      retry_count_{ traits::int32_count_type{} }
{
    add_modify(email_describe, email);
}

celeritas::traits::string_type celeritas::email_code::get_email() const
{
    return email_.get_value();
}

celeritas::traits::int32_type celeritas::email_code::get_code() const noexcept
{
    return code_.get_value();
}

celeritas::traits::int32_count_type celeritas::email_code::get_retry_count() const noexcept
{
    return retry_count_.get_value();
}

void celeritas::email_code::set_email(traits::param_type::string_type email)
{
    if (email != get_email())
    {
        email_.set_value(email);

        add_modify(email_describe, get_email());
    }
}

void celeritas::email_code::set_code(traits::param_type::int32_type code)
{
    if (code != get_code())
    {
        code_.set_value(code);

        add_modify(code_describe, get_code());
    }
}

void celeritas::email_code::set_retry_count(traits::param_type::int32_count_type retry_count)
{
    if (retry_count != get_retry_count())
    {
        retry_count_.set_value(retry_count);

        add_modify(retry_count_describe, get_retry_count());
    }
}

void celeritas::email_code::modify_retry_count(traits::param_type::int32_count_type retry_count)
{
    retry_count_.modify_value(retry_count);

    add_modify(retry_count_describe, get_retry_count());
}

const celeritas::database_entity::database_field_container& celeritas::email_code::get_database_field_container()
{
    static const database_field_container field_name_container{ decltype(email_)::get_database_field(),
                                                                decltype(code_)::get_database_field(),
                                                                decltype(retry_count_)::get_database_field() };

    return field_name_container;
}

celeritas::email_code::database_entity_change_const_shared_ptr celeritas::email_code::get_select(const database_type database_type)
{
    static const auto result = std::make_shared<database_entity_change>(database_type,
                                                                        database_name,
                                                                        database_change_type::select_type);

    return result;
}

celeritas::email_code::database_entity_change_const_shared_ptr celeritas::email_code::get_select(const database_type database_type, traits::param_type::string_type email)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    get_key_basis_database_container(database_type, email));
}

celeritas::email_code::database_entity_change_const_shared_ptr celeritas::email_code::get_select(const database_type database_type, const basis_database_container_const_shared_ptr& key)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    key);
}

celeritas::email_code::basis_database_container_const_shared_ptr celeritas::email_code::get_key_basis_database_container(const database_type database_type, traits::param_type::string_type email)
{
    const auto field_name = database_type == database_type::mongo ? "_id" : email_describe;

    const auto container = std::make_shared<basis_database_container>(basis_database{ field_name, email });

    return container;
}