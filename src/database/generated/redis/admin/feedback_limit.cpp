// 此文件是自动生成，请勿手动修改。

#include "feedback_limit.h"
#include "config/basic/database_type.h"
#include "database/basic/basis_database.tpp"
#include "database/basic/database_change_type.h"
#include "database/basic/database_entity_change.tpp"
#include "database/entity/database_entity.h"
#include "database/entity/entity.tpp"

celeritas::feedback_limit::feedback_limit(const database_entity_change& entity)
    : base_type{ entity.get_database_entity_change(ip_describe)  },
      ip_{ entity.get_value<database_data_type::string_type>(ip_describe) },
      count_{ entity.get_value<database_data_type::int32_count_type>(count_describe) }
{
}

celeritas::feedback_limit::feedback_limit(const database_type database_type, const database_entity_change& entity)
    : base_type{ database_type, entity.get_database_entity_change(ip_describe) },
      ip_{ entity.get_value<database_data_type::string_type>(ip_describe) },
      count_{ entity.get_value<database_data_type::int32_count_type>(count_describe) }
{
    if (database_type != entity.get_database_type())
    {
        add_modify(ip_describe, get_ip());
        add_modify(count_describe, get_count());
    }
}

celeritas::feedback_limit::feedback_limit(const database_type database_type, traits::param_type::string_type ip)
    : base_type{ database_type, database_name, get_key_basis_database_container(ip) },
      ip_{ ip },
      count_{ traits::int32_count_type{} }
{
    add_modify(ip_describe, ip);
}

celeritas::traits::string_type celeritas::feedback_limit::get_ip() const
{
    return ip_.get_value();
}

celeritas::traits::int32_count_type celeritas::feedback_limit::get_count() const noexcept
{
    return count_.get_value();
}

void celeritas::feedback_limit::set_ip(traits::param_type::string_type ip)
{
    if (ip != get_ip())
    {
        ip_.set_value(ip);

        add_modify(ip_describe, get_ip());
    }
}

void celeritas::feedback_limit::set_count(traits::param_type::int32_count_type count)
{
    if (count != get_count())
    {
        count_.set_value(count);

        add_modify(count_describe, get_count());
    }
}

void celeritas::feedback_limit::modify_count(traits::param_type::int32_count_type count)
{
    count_.modify_value(count);

    add_modify(count_describe, get_count());
}

const celeritas::database_entity::database_field_container& celeritas::feedback_limit::get_database_field_container()
{
    static const database_field_container field_name_container{ decltype(ip_)::get_database_field(),
                                                                decltype(count_)::get_database_field() };

    return field_name_container;
}

celeritas::feedback_limit::const_database_entity_change_shared_ptr celeritas::feedback_limit::get_select(const database_type database_type)
{
    static const auto result = std::make_shared<database_entity_change>(database_type,
                                                                        database_name,
                                                                        database_change_type::select_type);

    return result;
}

celeritas::feedback_limit::const_database_entity_change_shared_ptr celeritas::feedback_limit::get_select(const database_type database_type, traits::param_type::string_type ip)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    get_key_basis_database_container(ip));
}

celeritas::feedback_limit::const_database_entity_change_shared_ptr celeritas::feedback_limit::get_select(const database_type database_type, const const_basis_database_container_shared_ptr& key)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    key);
}

celeritas::feedback_limit::const_basis_database_container_shared_ptr celeritas::feedback_limit::get_key_basis_database_container(traits::param_type::string_type ip)
{
    return std::make_shared<basis_database_container>(basis_database{ ip_describe, ip });
}