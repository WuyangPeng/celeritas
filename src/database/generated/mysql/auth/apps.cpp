// 此文件是自动生成，请勿手动修改。

#include "apps.h"
#include "config/database_type.h"
#include "database/database_change_type.h"
#include "database/database_entity_change.tpp"
#include "database/database_entity.tpp"
#include "database/entity.tpp"

celeritas::apps::apps(const database_entity_change& entity)
    : base_type{ entity },
      app_id_{ entity.get_value<database_data_type::int64_type>(app_id_describe) },
      game_name_{ entity.get_value<database_data_type::string_type>(game_name_describe) },
      app_secret_{ entity.get_value<database_data_type::string_type>(app_secret_describe) },
      app_payment_secret_{ entity.get_value<database_data_type::string_type>(app_payment_secret_describe) },
      sms_provider_id_{ entity.get_value<database_data_type::int64_type>(sms_provider_id_describe) },
      email_provider_id_{ entity.get_value<database_data_type::int64_type>(email_provider_id_describe) },
      status_{ entity.get_value<database_data_type::int32_type>(status_describe) }
{
}

celeritas::apps::apps(const database_type database_type, const database_entity_change& entity)
    : base_type{ database_type, entity },
      app_id_{ entity.get_value<database_data_type::int64_type>(app_id_describe) },
      game_name_{ entity.get_value<database_data_type::string_type>(game_name_describe) },
      app_secret_{ entity.get_value<database_data_type::string_type>(app_secret_describe) },
      app_payment_secret_{ entity.get_value<database_data_type::string_type>(app_payment_secret_describe) },
      sms_provider_id_{ entity.get_value<database_data_type::int64_type>(sms_provider_id_describe) },
      email_provider_id_{ entity.get_value<database_data_type::int64_type>(email_provider_id_describe) },
      status_{ entity.get_value<database_data_type::int32_type>(status_describe) }
{
    if (database_type != entity.get_database_type())
    {
        add_modify(app_id_describe, get_app_id());
        add_modify(game_name_describe, get_game_name());
        add_modify(app_secret_describe, get_app_secret());
        add_modify(app_payment_secret_describe, get_app_payment_secret());
        add_modify(sms_provider_id_describe, get_sms_provider_id());
        add_modify(email_provider_id_describe, get_email_provider_id());
        add_modify(status_describe, get_status());
    }
}

celeritas::apps::apps(const database_type database_type, traits::param_type::int64_type app_id)
    : base_type{ database_type, database_name, get_key_basis_database_container(app_id) },
      app_id_{ app_id },
      game_name_{ traits::string_type{} },
      app_secret_{ traits::string_type{} },
      app_payment_secret_{ traits::string_type{} },
      sms_provider_id_{ traits::int64_type{} },
      email_provider_id_{ traits::int64_type{} },
      status_{ traits::int32_type{} }
{
    add_modify(app_id_describe, app_id);
}

celeritas::traits::int64_type celeritas::apps::get_app_id() const noexcept
{
    return app_id_.get_value();
}

celeritas::traits::string_type celeritas::apps::get_game_name() const
{
    return game_name_.get_value();
}

celeritas::traits::string_type celeritas::apps::get_app_secret() const
{
    return app_secret_.get_value();
}

celeritas::traits::string_type celeritas::apps::get_app_payment_secret() const
{
    return app_payment_secret_.get_value();
}

celeritas::traits::int64_type celeritas::apps::get_sms_provider_id() const noexcept
{
    return sms_provider_id_.get_value();
}

celeritas::traits::int64_type celeritas::apps::get_email_provider_id() const noexcept
{
    return email_provider_id_.get_value();
}

celeritas::traits::int32_type celeritas::apps::get_status() const noexcept
{
    return status_.get_value();
}

void celeritas::apps::set_app_id(traits::param_type::int64_type app_id)
{
    if (app_id != get_app_id())
    {
        app_id_.set_value(app_id);

        add_modify(app_id_describe, get_app_id());
    }
}

void celeritas::apps::set_game_name(traits::param_type::string_type game_name)
{
    if (game_name != get_game_name())
    {
        game_name_.set_value(game_name);

        add_modify(game_name_describe, get_game_name());
    }
}

void celeritas::apps::set_app_secret(traits::param_type::string_type app_secret)
{
    if (app_secret != get_app_secret())
    {
        app_secret_.set_value(app_secret);

        add_modify(app_secret_describe, get_app_secret());
    }
}

void celeritas::apps::set_app_payment_secret(traits::param_type::string_type app_payment_secret)
{
    if (app_payment_secret != get_app_payment_secret())
    {
        app_payment_secret_.set_value(app_payment_secret);

        add_modify(app_payment_secret_describe, get_app_payment_secret());
    }
}

void celeritas::apps::set_sms_provider_id(traits::param_type::int64_type sms_provider_id)
{
    if (sms_provider_id != get_sms_provider_id())
    {
        sms_provider_id_.set_value(sms_provider_id);

        add_modify(sms_provider_id_describe, get_sms_provider_id());
    }
}

void celeritas::apps::set_email_provider_id(traits::param_type::int64_type email_provider_id)
{
    if (email_provider_id != get_email_provider_id())
    {
        email_provider_id_.set_value(email_provider_id);

        add_modify(email_provider_id_describe, get_email_provider_id());
    }
}

void celeritas::apps::set_status(traits::param_type::int32_type status)
{
    if (status != get_status())
    {
        status_.set_value(status);

        add_modify(status_describe, get_status());
    }
}

const celeritas::database_entity::database_field_container& celeritas::apps::get_database_field_container()
{
    static const database_field_container field_name_container{ decltype(app_id_)::get_database_field(),
                                                                decltype(game_name_)::get_database_field(),
                                                                decltype(app_secret_)::get_database_field(),
                                                                decltype(app_payment_secret_)::get_database_field(),
                                                                decltype(sms_provider_id_)::get_database_field(),
                                                                decltype(email_provider_id_)::get_database_field(),
                                                                decltype(status_)::get_database_field() };

    return field_name_container;
}

celeritas::apps::database_entity_change_const_shared_ptr celeritas::apps::get_select(const database_type database_type)
{
    static const auto result = std::make_shared<database_entity_change>(database_type,
                                                                        database_name,
                                                                        database_change_type::select_type);

    return result;
}

celeritas::apps::database_entity_change_const_shared_ptr celeritas::apps::get_select(const database_type database_type, traits::param_type::int64_type app_id)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    get_key_basis_database_container(app_id));
}

celeritas::apps::database_entity_change_const_shared_ptr celeritas::apps::get_select(const database_type database_type, const basis_database_container_const_shared_ptr& key)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    key);
}

celeritas::apps::basis_database_container_const_shared_ptr celeritas::apps::get_key_basis_database_container(traits::param_type::int64_type app_id)
{
    return std::make_shared<basis_database_container>(basis_database{ app_id_describe, app_id });
}