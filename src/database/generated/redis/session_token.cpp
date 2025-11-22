// 此文件是自动生成，请勿手动修改。

#include "session_token.h"
#include "config/database_type.h"
#include "database/basis_database_manager.tpp"
#include "database/database_change_type.h"
#include "database/database_entity.tpp"
#include "database/entity.tpp"

celeritas::session_token celeritas::session_token::create(const basis_database_manager& entity, const database_type database_type, traits::param_type::string_type token)
{
    return entity.is_modify() ? session_token{ entity } : session_token{ database_type, token };
}

celeritas::session_token::session_token(const basis_database_manager& entity)
    : base_type{ entity },
      token_{ entity.get_value<database_data_type::string_type>(entity.get_database_type() == database_type::mongo ? "_id" : token_describe) },
      account_id_{ entity.get_value<database_data_type::int64_type>(account_id_describe) },
      expire_milliseconds_{ entity.get_value<database_data_type::int64_type>(expire_milliseconds_describe) }
{
}

celeritas::session_token::session_token(const database_type database_type, traits::param_type::string_type token)
    : base_type{ database_type, database_name.data(), get_key_basis_database_container(database_type, token) },
      token_{ token },
      account_id_{ traits::int64_type{} },
      expire_milliseconds_{ traits::int64_type{} }
{
    add_modify(token_describe, token);
}

celeritas::traits::string_type celeritas::session_token::get_token() const
{
    return token_.get_value();
}

celeritas::traits::int64_type celeritas::session_token::get_account_id() const noexcept
{
    return account_id_.get_value();
}

celeritas::traits::int64_type celeritas::session_token::get_expire_milliseconds() const noexcept
{
    return expire_milliseconds_.get_value();
}

void celeritas::session_token::set_token(traits::param_type::string_type token)
{
    if (token != get_token())
    {
        token_.set_value(token);

        add_modify(token_describe, get_token());
    }
}

void celeritas::session_token::set_account_id(traits::param_type::int64_type account_id)
{
    if (account_id != get_account_id())
    {
        account_id_.set_value(account_id);

        add_modify(account_id_describe, get_account_id());
    }
}

void celeritas::session_token::set_expire_milliseconds(traits::param_type::int64_type expire_milliseconds)
{
    if (expire_milliseconds != get_expire_milliseconds())
    {
        expire_milliseconds_.set_value(expire_milliseconds);

        add_modify(expire_milliseconds_describe, get_expire_milliseconds());
    }
}

const celeritas::database_entity::database_field_container& celeritas::session_token::get_database_field_container()
{
    static const database_field_container field_name_container{ decltype(token_)::get_database_field(),
                                                                decltype(account_id_)::get_database_field(),
                                                                decltype(expire_milliseconds_)::get_database_field() };

    return field_name_container;
}

celeritas::session_token::basis_database_manager_const_hared_ptr celeritas::session_token::get_select(const database_type database_type, traits::param_type::string_type token)
{
    return std::make_shared<basis_database_manager>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    get_key_basis_database_container(database_type, token));
}

celeritas::session_token::basis_database_manager_shared_ptr celeritas::session_token::get_select_all(const database_type database_type)
{
    static const auto result = std::make_shared<basis_database_manager>(database_type,
                                                                        database_name,
                                                                        database_change_type::select_type,
                                                                        basis_database_container{});

    return result;
}

celeritas::basis_database_container celeritas::session_token::get_key_basis_database_container(const database_type database_type, traits::param_type::string_type token)
{
    const auto field_name = database_type == database_type::mongo ? "_id" : token_describe;

    basis_database_container basis_database_container{ basis_database_container::object_container{ basis_database{ field_name, token } } };

    return basis_database_container;
}