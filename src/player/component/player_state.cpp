#include "player_null_component.h"
#include "player_state.tpp"
#include "player_state_type.h"
#include "common/core/celeritas_error.h"
#include "common/framework/resource_loader_base.h"
#include "common/logging/logger.h"
#include "initializer/initializer_constant.h"
#include "player/activity/player_activity_component.h"
#include "player/attribute/player_attribute_component.h"
#include "player/develop/player_develop_component.h"
#include "player/finish/player_finish_component.h"
#include "player/instance/player_instance_component.h"
#include "player/item/player_item_component.h"
#include "player/mail/player_mail_component.h"
#include "player/online/player_online_component.h"
#include "player/red_dot/player_red_dot_component.h"
#include "player/role/player_role_component.h"
#include "player/task/player_task_component.h"
#include "player/time/player_time_component.h"
#include "player/user/player_user_component.h"
#include "proto/celeritas.pb.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

celeritas::player_state::player_state(const user& user,
                                      const resource_loader_shared_ptr& resource_loader,
                                      const any_io_executor& any_io_executor,
                                      std::string instance_id,
                                      const service_login_request_type& login)
    : dirty_{ false },
      player_state_{ player_state_type::loading },
      components_{ std::make_shared<player_user_component>(user, this),
                   std::make_shared<player_role_component>(this, login),
                   std::make_shared<player_online_component>(this),
                   std::make_shared<player_time_component>(this),
                   std::make_shared<player_red_dot_component>(this),
                   std::make_shared<player_item_component>(this),
                   std::make_shared<player_develop_component>(this),
                   std::make_shared<player_mail_component>(this),
                   std::make_shared<player_task_component>(this),
                   std::make_shared<player_activity_component>(this),
                   std::make_shared<player_attribute_component>(this),
                   std::make_shared<player_instance_component>(this),

                   std::make_shared<player_finish_component>(this),
                   std::make_shared<player_null_component>(this) },
      resource_loader_{ resource_loader },
      instance_id_{ std::move(instance_id) },
      strand_{ boost::asio::make_strand(any_io_executor) }
{
    check();
}

celeritas::player_state::player_component_shared_ptr celeritas::player_state::get_component(player_component_type player_component_type) const
{
    return components_.at(static_cast<int>(player_component_type));
}

celeritas::player_state::void_awaitable_type celeritas::player_state::on_load_db()
{
    for (const auto& element : components_)
    {
        co_await element->on_load_db();
    }
}

celeritas::player_state::void_awaitable_type celeritas::player_state::on_db_analysis()
{
    for (const auto& element : components_)
    {
        co_await element->on_db_analysis();
    }
}

celeritas::player_state::void_awaitable_type celeritas::player_state::on_dependencies_ready()
{
    for (const auto& element : components_)
    {
        co_await element->on_dependencies_ready();
    }
}

celeritas::player_state::void_awaitable_type celeritas::player_state::send_initial_sync()
{
    for (const auto& element : components_)
    {
        co_await element->send_initial_sync();
    }
}

celeritas::player_state::void_awaitable_type celeritas::player_state::on_login()
{
    for (const auto& element : components_)
    {
        co_await element->on_login();
    }
}

celeritas::player_state::void_awaitable_type celeritas::player_state::on_logout()
{
    for (const auto& element : components_)
    {
        co_await element->on_logout();

        if (player_state_ == player_state_type::online)
        {
            co_await element->save_db();
        }
    }

    dirty_ = false;
    player_state_ = player_state_type::disconnected_ghost;
}

celeritas::player_state::void_awaitable_type celeritas::player_state::save_db()
{
    if (!dirty_ || player_state_ != player_state_type::online)
    {
        co_return;
    }

    for (const auto& element : components_)
    {
        if (element->is_modify())
        {
            co_await element->save_db();
        }
    }

    dirty_ = false;
}

void celeritas::player_state::set_dirty()
{
    dirty_ = true;
}

void celeritas::player_state::set_player_state_type(const player_state_type player_state_type)
{
    player_state_ = player_state_type;
}

celeritas::player_state_type celeritas::player_state::get_player_state_type() const
{
    return player_state_;
}

int64_t celeritas::player_state::get_user_id() const noexcept
{
    return get_component<player_user_component>()->get_user_id();
}

std::string celeritas::player_state::get_game_server_id() const
{
    return get_component<player_user_component>()->get_game_server_id();
}

std::string celeritas::player_state::generate_token()
{
    boost::uuids::random_generator generator{};
    const auto uuid = generator();

    return boost::uuids::to_string(uuid);
}

bool celeritas::player_state::write(const std::string& server_type, const std::string& instance_id, const header& header, const protobuf_message& request)
{
    const auto resource_loader_shared_ptr = resource_loader_.lock();
    if (resource_loader_shared_ptr != nullptr)
    {
        return resource_loader_shared_ptr->write_to_server(server_type, instance_id, header, request);
    }

    return false;
}

std::string celeritas::player_state::get_instance_id() const
{
    return instance_id_;
}

void celeritas::player_state::set_instance_id(const std::string& instance_id)
{
    instance_id_ = instance_id;
}

celeritas::player_state::void_awaitable_type celeritas::player_state::time_callback(const player_time_refresh_key& player_time_refresh_key, const bool is_login) const
{
    co_await get_component<player_time_component>()->time_callback(player_time_refresh_key, is_login);
}

void celeritas::player_state::set_mock_player_component(const player_component_shared_ptr& mock)
{
    components_.at(static_cast<int>(player_component_type::mock)) = mock;
}

celeritas::player_state::void_awaitable_type celeritas::player_state::set_login(const std::string& instance_id, const service_login_request_type& login)
{
    set_instance_id(instance_id);
    get_component<player_role_component>()->set_login(login);

    co_return;
}

celeritas::player_state::any_io_executor celeritas::player_state::get_any_io_executor()
{
    return strand_;
}

void celeritas::player_state::send_error_message(const int rpc, const game_error_type game_error_type)
{
    const header header{ rpc, get_user_id(), game_error_type };

    proto::celeritas response{};
    response.mutable_celeritas_response()->mutable_client()->mutable_player()->mutable_error();
    if (!write(gateway_type.data(), get_instance_id(), header, response))
    {
        LOG_CHANNEL(player_channel, error) << "send message error.";
    }
}

void celeritas::player_state::check() const
{
    for (auto index = 0; index < static_cast<int>(player_component_type::max_component); ++index)
    {
        if (static_cast<int>(components_.at(index)->get_player_component_type()) != index)
        {
            throw celeritas_error{ "component type error,index = {}", index };
        }
    }
}