// 此文件是自动生成，请勿手动修改。

#include "sms_providers.h"
#include "config/database_type.h"
#include "database/database_change_type.h"
#include "database/database_entity_change.tpp"
#include "database/database_entity.tpp"
#include "database/entity.tpp"

celeritas::sms_providers::sms_providers(const database_entity_change& entity)
    : base_type{ entity },
      provider_id_{ entity.get_value<database_data_type::int64_type>(provider_id_describe) },
      provider_name_{ entity.get_value<database_data_type::string_type>(provider_name_describe) },
      base_url_{ entity.get_value<database_data_type::string_type>(base_url_describe) },
      api_key_{ entity.get_value<database_data_type::string_type>(api_key_describe) },
      api_secret_{ entity.get_value<database_data_type::string_type>(api_secret_describe) },
      decryption_key_{ entity.get_value<database_data_type::string_type>(decryption_key_describe) },
      active_{ entity.get_value<database_data_type::bool_type>(active_describe) },
      process_type_{ entity.get_value<database_data_type::int32_type>(process_type_describe) }
{
}

celeritas::sms_providers::sms_providers(const database_type database_type, const database_entity_change& entity)
    : base_type{ database_type, entity },
      provider_id_{ entity.get_value<database_data_type::int64_type>(provider_id_describe) },
      provider_name_{ entity.get_value<database_data_type::string_type>(provider_name_describe) },
      base_url_{ entity.get_value<database_data_type::string_type>(base_url_describe) },
      api_key_{ entity.get_value<database_data_type::string_type>(api_key_describe) },
      api_secret_{ entity.get_value<database_data_type::string_type>(api_secret_describe) },
      decryption_key_{ entity.get_value<database_data_type::string_type>(decryption_key_describe) },
      active_{ entity.get_value<database_data_type::bool_type>(active_describe) },
      process_type_{ entity.get_value<database_data_type::int32_type>(process_type_describe) }
{
    if (database_type != entity.get_database_type())
    {
        add_modify(provider_id_describe, get_provider_id());
        add_modify(provider_name_describe, get_provider_name());
        add_modify(base_url_describe, get_base_url());
        add_modify(api_key_describe, get_api_key());
        add_modify(api_secret_describe, get_api_secret());
        add_modify(decryption_key_describe, get_decryption_key());
        add_modify(active_describe, is_active());
        add_modify(process_type_describe, get_process_type());
    }
}

celeritas::sms_providers::sms_providers(const database_type database_type, traits::param_type::int64_type provider_id)
    : base_type{ database_type, database_name, get_key_basis_database_container(database_type, provider_id) },
      provider_id_{ provider_id },
      provider_name_{ traits::string_type{} },
      base_url_{ traits::string_type{} },
      api_key_{ traits::string_type{} },
      api_secret_{ traits::string_type{} },
      decryption_key_{ traits::string_type{} },
      active_{ traits::bool_type{} },
      process_type_{ traits::int32_type{} }
{
    add_modify(provider_id_describe, provider_id);
}

celeritas::traits::int64_type celeritas::sms_providers::get_provider_id() const noexcept
{
    return provider_id_.get_value();
}

celeritas::traits::string_type celeritas::sms_providers::get_provider_name() const
{
    return provider_name_.get_value();
}

celeritas::traits::string_type celeritas::sms_providers::get_base_url() const
{
    return base_url_.get_value();
}

celeritas::traits::string_type celeritas::sms_providers::get_api_key() const
{
    return api_key_.get_value();
}

celeritas::traits::string_type celeritas::sms_providers::get_api_secret() const
{
    return api_secret_.get_value();
}

celeritas::traits::string_type celeritas::sms_providers::get_decryption_key() const
{
    return decryption_key_.get_value();
}

celeritas::traits::bool_type celeritas::sms_providers::is_active() const noexcept
{
    return active_.get_value();
}

celeritas::traits::int32_type celeritas::sms_providers::get_process_type() const noexcept
{
    return process_type_.get_value();
}

void celeritas::sms_providers::set_provider_id(traits::param_type::int64_type provider_id)
{
    if (provider_id != get_provider_id())
    {
        provider_id_.set_value(provider_id);

        add_modify(provider_id_describe, get_provider_id());
    }
}

void celeritas::sms_providers::set_provider_name(traits::param_type::string_type provider_name)
{
    if (provider_name != get_provider_name())
    {
        provider_name_.set_value(provider_name);

        add_modify(provider_name_describe, get_provider_name());
    }
}

void celeritas::sms_providers::set_base_url(traits::param_type::string_type base_url)
{
    if (base_url != get_base_url())
    {
        base_url_.set_value(base_url);

        add_modify(base_url_describe, get_base_url());
    }
}

void celeritas::sms_providers::set_api_key(traits::param_type::string_type api_key)
{
    if (api_key != get_api_key())
    {
        api_key_.set_value(api_key);

        add_modify(api_key_describe, get_api_key());
    }
}

void celeritas::sms_providers::set_api_secret(traits::param_type::string_type api_secret)
{
    if (api_secret != get_api_secret())
    {
        api_secret_.set_value(api_secret);

        add_modify(api_secret_describe, get_api_secret());
    }
}

void celeritas::sms_providers::set_decryption_key(traits::param_type::string_type decryption_key)
{
    if (decryption_key != get_decryption_key())
    {
        decryption_key_.set_value(decryption_key);

        add_modify(decryption_key_describe, get_decryption_key());
    }
}

void celeritas::sms_providers::set_active(traits::param_type::bool_type active)
{
    if (active != is_active())
    {
        active_.set_value(active);

        add_modify(active_describe, is_active());
    }
}

void celeritas::sms_providers::set_process_type(traits::param_type::int32_type process_type)
{
    if (process_type != get_process_type())
    {
        process_type_.set_value(process_type);

        add_modify(process_type_describe, get_process_type());
    }
}

const celeritas::database_entity::database_field_container& celeritas::sms_providers::get_database_field_container()
{
    static const database_field_container field_name_container{ decltype(provider_id_)::get_database_field(),
                                                                decltype(provider_name_)::get_database_field(),
                                                                decltype(base_url_)::get_database_field(),
                                                                decltype(api_key_)::get_database_field(),
                                                                decltype(api_secret_)::get_database_field(),
                                                                decltype(decryption_key_)::get_database_field(),
                                                                decltype(active_)::get_database_field(),
                                                                decltype(process_type_)::get_database_field() };

    return field_name_container;
}

celeritas::sms_providers::database_entity_change_const_shared_ptr celeritas::sms_providers::get_select(const database_type database_type)
{
    static const auto result = std::make_shared<database_entity_change>(database_type,
                                                                        database_name,
                                                                        database_change_type::select_type);

    return result;
}

celeritas::sms_providers::database_entity_change_const_shared_ptr celeritas::sms_providers::get_select(const database_type database_type, traits::param_type::int64_type provider_id)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    get_key_basis_database_container(database_type, provider_id));
}

celeritas::sms_providers::database_entity_change_const_shared_ptr celeritas::sms_providers::get_select(const database_type database_type, const basis_database_container_const_shared_ptr& key)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    key);
}

celeritas::sms_providers::basis_database_container_const_shared_ptr celeritas::sms_providers::get_key_basis_database_container(const database_type database_type, traits::param_type::int64_type provider_id)
{
    return std::make_shared<basis_database_container>(basis_database{ provider_id_describe, provider_id });
}