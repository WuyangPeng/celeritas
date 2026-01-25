#include "player_role_component.h"
#include "common/core/time_helper.h"
#include "common/logging/logger.h"
#include "config/basic/database_type.h"
#include "config/game/game_config.h"
#include "config/game/game_tables.h"
#include "config/luban/generated/schema.h"
#include "database/generated/mongo/auth/user_server_roles.h"
#include "database/pool/database_pool_base.h"
#include "initializer/initializer_constant.h"
#include "player/component/player_state.tpp"
#include "player/time/player_time_refresh_key.h"
#include "player/time/time_refresh_type.h"
#include "player/user/player_user_component.h"
#include "proto/celeritas.pb.h"

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
        if (const auto mongo_player_pool = get_mongo_player_database_pool();
            co_await mongo_player_pool->execute_changes(user_role_->get_modify()))
        {
            user_role_->clear_modify();
        }
    }

    if (user_server_roles_->is_must_save())
    {
        if (const auto mongo_auth_pool = get_mongo_auth_database_pool();
            co_await mongo_auth_pool->execute_changes(user_server_roles_->get_modify()))
        {
            user_server_roles_->clear_modify();
        }
    }
}

bool celeritas::player_role_component::is_modify() const
{
    return user_role_->is_must_save() || user_server_roles_->is_must_save();
}

celeritas::player_role_component::bool_awaitable_type celeritas::player_role_component::change_name(const std::string& surname, const std::string& name)
{
    const auto mongo_player_pool = get_mongo_player_database_pool();
    if (!co_await mongo_player_pool->execute_changes(user_role_->get_modify()))
    {
        co_return false;
    }

    const auto change_name_time = time_helper::get_current_milliseconds();
    const auto player_user = get_player_state()->get_component<player_user_component>();
    const auto old_user_role = *user_role_;

    user_role_->set_surname(surname);
    user_role_->set_name(name);
    user_role_->set_modify_name(true);
    user_role_->set_change_name_time(change_name_time);
    user_role_->set_full_name(player_user->get_game_server_id() + surname + name);

    if (co_await mongo_player_pool->execute_changes(user_role_->get_modify()))
    {
        user_role_->clear_modify();
    }
    else
    {
        user_role_ = old_user_role;
        user_role_->clear_modify();
        co_return false;
    }

    server_role_->set_role_surname(surname);
    server_role_->set_role_name(name);
    user_server_roles_->set_servers(server_role_index_, server_role_->to_document_type());
    user_server_roles_->set_update_time(change_name_time);

    get_player_state()->set_dirty();

    co_return true;
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

celeritas::player_component::void_awaitable_type celeritas::player_role_component::time_callback(const player_time_refresh_key& player_time_refresh_key, bool is_login)
{
    if (player_time_refresh_key.get_time_refresh_type() == time_refresh_type::daily)
    {
        user_role_->set_per_day_change_count(0);

        get_player_state()->set_dirty();

        if (!is_login)
        {
            send_role_response();
        }
    }

    co_return;
}

celeritas::player_component::void_awaitable_type celeritas::player_role_component::send_initial_sync()
{
    send_role_response();

    co_return;
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
        user_role_->set_change_name_time(time_helper::get_current_milliseconds());
        user_role_->set_full_name(std::to_string(user_id));
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

void celeritas::player_role_component::send_role_response()
{
    auto* player_state = get_player_state();

    const header header{ player_state->get_user_id() };

    proto::celeritas response{};
    auto* role_response = response.mutable_celeritas_response()->mutable_client()->mutable_player()->mutable_role()->mutable_role();

    role_response->set_surname(user_role_->get_surname());
    role_response->set_name(user_role_->get_name());
    role_response->set_modify_name(user_role_->is_modify());
    role_response->set_change_count(user_role_->get_change_count());
    role_response->set_per_day_change_count(user_role_->get_per_day_change_count());

    if (!player_state->write(gateway_type.data(), player_state->get_instance_id(), header, response))
    {
        LOG_CHANNEL(player_channel, error) << "send message error.";
    }
}
