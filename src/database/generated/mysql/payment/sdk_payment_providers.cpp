// 此文件是自动生成，请勿手动修改。

#include "sdk_payment_providers.h"
#include "config/database_type.h"
#include "database/database_change_type.h"
#include "database/database_entity_change.tpp"
#include "database/database_entity.tpp"
#include "database/entity.tpp"

celeritas::sdk_payment_providers::sdk_payment_providers(const database_entity_change& entity)
    : base_type{ entity },
      sdk_id_{ entity.get_value<database_data_type::int64_type>(entity.get_database_type() == database_type::mongo ? "_id" : sdk_id_describe) },
      app_id_{ entity.get_value<database_data_type::int64_type>(app_id_describe) },
      platform_{ entity.get_value<database_data_type::int32_type>(platform_describe) },
      provider_name_{ entity.get_value<database_data_type::string_type>(provider_name_describe) },
      api_key_{ entity.get_value<database_data_type::string_type>(api_key_describe) },
      api_secret_{ entity.get_value<database_data_type::string_type>(api_secret_describe) },
      decryption_key_{ entity.get_value<database_data_type::string_type>(decryption_key_describe) },
      http_suffix_{ entity.get_value<database_data_type::string_type>(http_suffix_describe) },
      active_{ entity.get_value<database_data_type::bool_type>(active_describe) }
{
}

celeritas::sdk_payment_providers::sdk_payment_providers(const database_type database_type, const database_entity_change& entity)
    : base_type{ database_type, entity },
      sdk_id_{ entity.get_value<database_data_type::int64_type>(entity.get_database_type() == database_type::mongo ? "_id" : sdk_id_describe) },
      app_id_{ entity.get_value<database_data_type::int64_type>(app_id_describe) },
      platform_{ entity.get_value<database_data_type::int32_type>(platform_describe) },
      provider_name_{ entity.get_value<database_data_type::string_type>(provider_name_describe) },
      api_key_{ entity.get_value<database_data_type::string_type>(api_key_describe) },
      api_secret_{ entity.get_value<database_data_type::string_type>(api_secret_describe) },
      decryption_key_{ entity.get_value<database_data_type::string_type>(decryption_key_describe) },
      http_suffix_{ entity.get_value<database_data_type::string_type>(http_suffix_describe) },
      active_{ entity.get_value<database_data_type::bool_type>(active_describe) }
{
    if (database_type != entity.get_database_type())
    {
        add_modify(sdk_id_describe, get_sdk_id());
        add_modify(app_id_describe, get_app_id());
        add_modify(platform_describe, get_platform());
        add_modify(provider_name_describe, get_provider_name());
        add_modify(api_key_describe, get_api_key());
        add_modify(api_secret_describe, get_api_secret());
        add_modify(decryption_key_describe, get_decryption_key());
        add_modify(http_suffix_describe, get_http_suffix());
        add_modify(active_describe, is_active());
    }
}

celeritas::sdk_payment_providers::sdk_payment_providers(const database_type database_type, traits::param_type::int64_type sdk_id)
    : base_type{ database_type, database_name, get_key_basis_database_container(database_type, sdk_id) },
      sdk_id_{ sdk_id },
      app_id_{ traits::int64_type{} },
      platform_{ traits::int32_type{} },
      provider_name_{ traits::string_type{} },
      api_key_{ traits::string_type{} },
      api_secret_{ traits::string_type{} },
      decryption_key_{ traits::string_type{} },
      http_suffix_{ traits::string_type{} },
      active_{ traits::bool_type{} }
{
    add_modify(sdk_id_describe, sdk_id);
}

celeritas::traits::int64_type celeritas::sdk_payment_providers::get_sdk_id() const noexcept
{
    return sdk_id_.get_value();
}

celeritas::traits::int64_type celeritas::sdk_payment_providers::get_app_id() const noexcept
{
    return app_id_.get_value();
}

celeritas::traits::int32_type celeritas::sdk_payment_providers::get_platform() const noexcept
{
    return platform_.get_value();
}

celeritas::traits::string_type celeritas::sdk_payment_providers::get_provider_name() const
{
    return provider_name_.get_value();
}

celeritas::traits::string_type celeritas::sdk_payment_providers::get_api_key() const
{
    return api_key_.get_value();
}

celeritas::traits::string_type celeritas::sdk_payment_providers::get_api_secret() const
{
    return api_secret_.get_value();
}

celeritas::traits::string_type celeritas::sdk_payment_providers::get_decryption_key() const
{
    return decryption_key_.get_value();
}

celeritas::traits::string_type celeritas::sdk_payment_providers::get_http_suffix() const
{
    return http_suffix_.get_value();
}

celeritas::traits::bool_type celeritas::sdk_payment_providers::is_active() const noexcept
{
    return active_.get_value();
}

void celeritas::sdk_payment_providers::set_sdk_id(traits::param_type::int64_type sdk_id)
{
    if (sdk_id != get_sdk_id())
    {
        sdk_id_.set_value(sdk_id);

        add_modify(sdk_id_describe, get_sdk_id());
    }
}

void celeritas::sdk_payment_providers::set_app_id(traits::param_type::int64_type app_id)
{
    if (app_id != get_app_id())
    {
        app_id_.set_value(app_id);

        add_modify(app_id_describe, get_app_id());
    }
}

void celeritas::sdk_payment_providers::set_platform(traits::param_type::int32_type platform)
{
    if (platform != get_platform())
    {
        platform_.set_value(platform);

        add_modify(platform_describe, get_platform());
    }
}

void celeritas::sdk_payment_providers::set_provider_name(traits::param_type::string_type provider_name)
{
    if (provider_name != get_provider_name())
    {
        provider_name_.set_value(provider_name);

        add_modify(provider_name_describe, get_provider_name());
    }
}

void celeritas::sdk_payment_providers::set_api_key(traits::param_type::string_type api_key)
{
    if (api_key != get_api_key())
    {
        api_key_.set_value(api_key);

        add_modify(api_key_describe, get_api_key());
    }
}

void celeritas::sdk_payment_providers::set_api_secret(traits::param_type::string_type api_secret)
{
    if (api_secret != get_api_secret())
    {
        api_secret_.set_value(api_secret);

        add_modify(api_secret_describe, get_api_secret());
    }
}

void celeritas::sdk_payment_providers::set_decryption_key(traits::param_type::string_type decryption_key)
{
    if (decryption_key != get_decryption_key())
    {
        decryption_key_.set_value(decryption_key);

        add_modify(decryption_key_describe, get_decryption_key());
    }
}

void celeritas::sdk_payment_providers::set_http_suffix(traits::param_type::string_type http_suffix)
{
    if (http_suffix != get_http_suffix())
    {
        http_suffix_.set_value(http_suffix);

        add_modify(http_suffix_describe, get_http_suffix());
    }
}

void celeritas::sdk_payment_providers::set_active(traits::param_type::bool_type active)
{
    if (active != is_active())
    {
        active_.set_value(active);

        add_modify(active_describe, is_active());
    }
}

const celeritas::database_entity::database_field_container& celeritas::sdk_payment_providers::get_database_field_container()
{
    static const database_field_container field_name_container{ decltype(sdk_id_)::get_database_field(),
                                                                decltype(app_id_)::get_database_field(),
                                                                decltype(platform_)::get_database_field(),
                                                                decltype(provider_name_)::get_database_field(),
                                                                decltype(api_key_)::get_database_field(),
                                                                decltype(api_secret_)::get_database_field(),
                                                                decltype(decryption_key_)::get_database_field(),
                                                                decltype(http_suffix_)::get_database_field(),
                                                                decltype(active_)::get_database_field() };

    return field_name_container;
}

celeritas::sdk_payment_providers::database_entity_change_const_shared_ptr celeritas::sdk_payment_providers::get_select(const database_type database_type)
{
    static const auto result = std::make_shared<database_entity_change>(database_type,
                                                                        database_name,
                                                                        database_change_type::select_type);

    return result;
}

celeritas::sdk_payment_providers::database_entity_change_const_shared_ptr celeritas::sdk_payment_providers::get_select(const database_type database_type, traits::param_type::int64_type sdk_id)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    get_key_basis_database_container(database_type, sdk_id));
}

celeritas::sdk_payment_providers::database_entity_change_const_shared_ptr celeritas::sdk_payment_providers::get_select(const database_type database_type, const basis_database_container_const_shared_ptr& key)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    key);
}

celeritas::sdk_payment_providers::basis_database_container_const_shared_ptr celeritas::sdk_payment_providers::get_key_basis_database_container(const database_type database_type, traits::param_type::int64_type sdk_id)
{
    const auto field_name = database_type == database_type::mongo ? "_id" : sdk_id_describe;

    const auto container = std::make_shared<basis_database_container>(basis_database{ field_name, sdk_id });

    return container;
}