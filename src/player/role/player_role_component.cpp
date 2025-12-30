#include "player_role_component.h"
#include "common/core/time_helper.h"
#include "config/database_type.h"
#include "config/game_config/game_config.h"
#include "config/game_config/game_tables.h"
#include "config/luban/generated/schema.h"
#include "database/database_pool_base.h"
#include "database/generated/mongo/auth/user_server_roles.h"
#include "player/component/player_state.tpp"
#include "player/user/player_user_component.h"

celeritas::player_role_component::player_role_component(player_state* player_state, const service_login_request_type& login) noexcept
    : base_type{ get_player_component_type(), player_state },
      user_role_{},
      user_server_roles_{},
      server_role_index_{},
      server_role_{},
      device_id_{ login.device_id() },
      app_version_{ login.app_version() }
{
}

celeritas::player_component::void_awaitable_type celeritas::player_role_component::on_load_db()
{
    co_await load_user_role_db();
    co_await load_user_server_roles_db();

    set_server_role();
}

celeritas::player_component::void_awaitable_type celeritas::player_role_component::save_db()
{
    if (user_role_->is_must_save())
    {
        const auto mongo_player_pool = get_mongo_player_database_pool();
        co_await mongo_player_pool->execute_changes(user_role_->get_modify());
        user_role_->clear_modify();
    }

    if (user_server_roles_->is_must_save())
    {
        const auto mongo_auth_pool = get_mongo_auth_database_pool();
        co_await mongo_auth_pool->execute_changes(user_server_roles_->get_modify());
        user_server_roles_->clear_modify();
    }
}

bool celeritas::player_role_component::is_modify() const
{
    return user_role_->is_must_save() || user_server_roles_->is_must_save();
}

void celeritas::player_role_component::change_name(const std::string& name)
{
    user_role_->set_name(name);
    server_role_->set_role_name(name);
    user_server_roles_->set_servers(server_role_index_, server_role_->to_document_type());
    user_server_roles_->set_update_time(time_helper::get_current_milliseconds());

    get_player_state()->set_dirty();
}

void celeritas::player_role_component::set_login(const service_login_request_type& login)
{
    user_role_->set_device_id(login.device_id());
    user_role_->set_app_version(login.app_version());
}

std::string celeritas::player_role_component::get_name() const
{
    return user_role_->get_name();
}

std::string celeritas::player_role_component::get_device_id() const
{
    return user_role_->get_device_id();
}

std::string celeritas::player_role_component::get_app_version() const
{
    return user_role_->get_app_version();
}

celeritas::player_role_component::void_awaitable_type celeritas::player_role_component::load_user_role_db()
{
    const auto mongo_player_pool = get_mongo_player_database_pool();
    const auto player_user = get_player_state()->get_component<player_user_component>();
    const auto user_id = player_user->get_user_id();

    if (const auto database_entity_change = co_await mongo_player_pool->select_one(user_role::get_select(database_type::mongo, user_id), user_role::get_database_field_container()))
    {
        user_role_ = user_role{ *database_entity_change };
    }
    else
    {
        user_role_ = user_role{ database_type::mongo, user_id };
        const auto game_tables = game_config::get_instance().get_game_tables();

        user_role_->set_surname(game_tables->get_surname());
        user_role_->set_name(game_tables->get_name(config::sex_type::none));
    }

    user_role_->set_device_id(device_id_);
    user_role_->set_app_version(app_version_);
}

celeritas::player_role_component::void_awaitable_type celeritas::player_role_component::load_user_server_roles_db()
{
    const auto mongo_auth_pool = get_mongo_auth_database_pool();

    const auto player_user = get_player_state()->get_component<player_user_component>();
    const auto account_id = player_user->get_account_id();

    if (const auto database_entity_change = co_await mongo_auth_pool->select_one(user_server_roles::get_select(database_type::mongo, account_id), user_server_roles::get_database_field_container()))
    {
        user_server_roles_ = user_server_roles{ *database_entity_change };
    }
    else
    {
        user_server_roles_ = user_server_roles{ database_type::mongo, account_id };
    }
}

void celeritas::player_role_component::set_server_role()
{
    const auto player_user = get_player_state()->get_component<player_user_component>();
    for (const auto servers = user_server_roles_->get_servers();
         const auto& element : servers)
    {
        if (auto server_role = server_role::from_document(element);
            server_role.get_game_server_id() == player_user->get_game_server_id())
        {
            server_role_ = server_role;

            break;
        }

        ++server_role_index_;
    }

    if (!server_role_)
    {
        server_role_ = server_role{ player_user->get_game_server_id(), user_role_->get_surname(), user_role_->get_name() };

        user_server_roles_->add_servers(server_role_->to_document_type());
        user_server_roles_->set_update_time(time_helper::get_current_milliseconds());
    }
}
