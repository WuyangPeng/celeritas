// 此文件是自动生成，请勿手动修改。

#include "admin_session_token.h"
#include "config/basic/database_type.h"
#include "database/basic/basis_database.tpp"
#include "database/basic/database_change_type.h"
#include "database/basic/database_entity_change.tpp"
#include "database/entity/database_entity.h"
#include "database/entity/entity.tpp"

celeritas::admin_session_token::admin_session_token(const database_entity_change& entity)
    : base_type{ entity.get_database_entity_change(token_describe)  },
      token_{ entity.get_value<database_data_type::string_type>(token_describe) },
      account_id_{ entity.get_value<database_data_type::int64_type>(account_id_describe) }
{
}

celeritas::admin_session_token::admin_session_token(const database_type database_type, const database_entity_change& entity)
    : base_type{ database_type, entity.get_database_entity_change(token_describe) },
      token_{ entity.get_value<database_data_type::string_type>(token_describe) },
      account_id_{ entity.get_value<database_data_type::int64_type>(account_id_describe) }
{
    if (database_type != entity.get_database_type())
    {
        add_modify(token_describe, get_token());
        add_modify(account_id_describe, get_account_id());
    }
}

celeritas::admin_session_token::admin_session_token(const database_type database_type, traits::param_type::string_type token)
    : base_type{ database_type, database_name, get_key_basis_database_container(token) },
      token_{ token },
      account_id_{ traits::int64_type{} }
{
    add_modify(token_describe, token);
}

celeritas::traits::string_type celeritas::admin_session_token::get_token() const
{
    return token_.get_value();
}

celeritas::traits::int64_type celeritas::admin_session_token::get_account_id() const noexcept
{
    return account_id_.get_value();
}

void celeritas::admin_session_token::set_token(traits::param_type::string_type token)
{
    if (token != get_token())
    {
        token_.set_value(token);

        add_modify(token_describe, get_token());
    }
}

void celeritas::admin_session_token::set_account_id(traits::param_type::int64_type account_id)
{
    if (account_id != get_account_id())
    {
        account_id_.set_value(account_id);

        add_modify(account_id_describe, get_account_id());
    }
}

const celeritas::database_entity::database_field_container& celeritas::admin_session_token::get_database_field_container()
{
    static const database_field_container field_name_container{ decltype(token_)::get_database_field(),
                                                                decltype(account_id_)::get_database_field() };

    return field_name_container;
}

celeritas::admin_session_token::const_database_entity_change_shared_ptr celeritas::admin_session_token::get_select(const database_type database_type)
{
    static const auto result = std::make_shared<database_entity_change>(database_type,
                                                                        database_name,
                                                                        database_change_type::select_type);

    return result;
}

celeritas::admin_session_token::const_database_entity_change_shared_ptr celeritas::admin_session_token::get_select(const database_type database_type, traits::param_type::string_type token)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    get_key_basis_database_container(token));
}

celeritas::admin_session_token::const_database_entity_change_shared_ptr celeritas::admin_session_token::get_select(const database_type database_type, const const_basis_database_container_shared_ptr& key)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    key);
}

celeritas::admin_session_token::const_basis_database_container_shared_ptr celeritas::admin_session_token::get_key_basis_database_container(traits::param_type::string_type token)
{
    return std::make_shared<basis_database_container>(basis_database{ token_describe, token });
}