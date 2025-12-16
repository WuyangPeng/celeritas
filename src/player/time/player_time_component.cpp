#include "player_time_component.h"
#include "common/logger.h"
#include "common/time_helper.h"
#include "config/database_type.h"
#include "database/database_pool_base.h"
#include "detail/player_timer.h"
#include "player/component/player_state.h"

#include <boost/polymorphic_pointer_cast.hpp>

celeritas::player_time_component::player_time_component(player_state* player_state) noexcept
    : base_type{ get_player_component_type(), player_state }, user_time_refresh_{}, player_time_refresh_{}, next_refresh_time_{}, player_timer_{}
{
}

celeritas::player_component::void_awaitable_type celeritas::player_time_component::on_load_db()
{
    const auto mongo_pool = get_mongo_player_db_name();
    const auto user_id = get_player_state()->get_user_id();

    if (const auto optional_user_time_refresh = co_await mongo_pool->select_one(user_time_refresh::get_select(database_type::mongo, user_id), user_time_refresh::get_database_field_container()))
    {
        user_time_refresh_ = user_time_refresh{ *optional_user_time_refresh };
    }
    else
    {
        user_time_refresh_ = user_time_refresh{ database_type::mongo, user_id };
    }

    for (const auto& element : user_time_refresh_->get_player_time_refresh())
    {
        player_time_refresh_.emplace_back(player_time_refresh::from_json_string(element));
    }
}

void celeritas::player_time_component::init_player_timer(const int64_t current_milliseconds)
{
    if (next_refresh_time_ > 0)
    {
        std::chrono::milliseconds duration{ next_refresh_time_ - current_milliseconds };
        player_timer_ = std::make_unique<player_timer>(get_player_state()->get_io_context(), duration, boost::polymorphic_pointer_cast<class_type>(shared_from_this()));

        player_timer_->start();
    }
}

celeritas::player_component::void_awaitable_type celeritas::player_time_component::on_dependencies_ready()
{
    const auto current_milliseconds = time_helper::get_current_milliseconds();
    for (auto& element : player_time_refresh_)
    {
        if (element.is_can_refresh())
        {
            for (const auto& component : element.get_component())
            {
                co_await get_player_state()->get_component(component)->time_callback(element.get_time_refresh_type(), element.get_parameter(), true);
            }

            element.set_last_refresh_time(current_milliseconds);
        }
    }

    calculate_next_refresh_time();

    init_player_timer(current_milliseconds);

    co_return;
}

celeritas::player_component::void_awaitable_type celeritas::player_time_component::save_db()
{
    if (user_time_refresh_->is_must_save())
    {
        const auto mongo_pool = get_mongo_player_db_name();

        co_await mongo_pool->execute_changes(user_time_refresh_->get_modify());

        user_time_refresh_->clear_modify();
    }
}

bool celeritas::player_time_component::is_modify() const
{
    return user_time_refresh_->is_must_save();
}

celeritas::player_component::void_awaitable_type celeritas::player_time_component::time_callback()
{
    try
    {
        co_await do_time_callback();
        calculate_next_refresh_time();
        wait_for_next_tick();
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(player_channel, error) << "error:" << error.what();
    }
    catch (...)
    {
        LOG_CHANNEL(player_channel, fatal) << "unknown error.";
    }

    co_return;
}

celeritas::player_component::void_awaitable_type celeritas::player_time_component::time_callback(const time_refresh_type time_refresh_type, const int64_t parameter, const bool is_login)
{
    const auto current_milliseconds = time_helper::get_current_milliseconds();
    for (auto& element : player_time_refresh_)
    {
        if (element.get_time_refresh_type() == time_refresh_type &&
            element.get_parameter() == parameter &&
            element.is_can_refresh())
        {
            for (const auto& component : element.get_component())
            {
                co_await get_player_state()->get_component(component)->time_callback(element.get_time_refresh_type(), element.get_parameter(), is_login);
            }

            element.set_last_refresh_time(current_milliseconds);
        }
    }
    calculate_next_refresh_time();

    co_return;
}

void celeritas::player_time_component::register_timer(const player_component_type player_component, const time_refresh_type time_refresh_type, const int64_t parameter)
{
    for (auto& element : player_time_refresh_)
    {
        if (element.get_time_refresh_type() == time_refresh_type &&
            element.get_parameter() == parameter)
        {
            const auto component = element.get_component();
            if (const auto iter = std::ranges::find(component, player_component);
                iter != component.end())
            {
                return;
            }

            element.add_component(player_component);
            return;
        }
    }

    player_time_refresh_.emplace_back(time_refresh_type, parameter, player_component);

    const auto old_next_refresh_time = get_next_refresh_time();
    calculate_next_refresh_time();
    if (old_next_refresh_time != get_next_refresh_time())
    {
        if (player_timer_ != nullptr)
        {
            player_timer_->stop();
            player_timer_->wait_for_next_tick();
        }
        else
        {
            init_player_timer(time_helper::get_current_milliseconds());
        }
    }
}

void celeritas::player_time_component::remove_timer(player_component_type player_component, time_refresh_type time_refresh_type, int64_t parameter)
{
    if (std::erase_if(player_time_refresh_,
                      [&](auto& element) {
                          if (element.get_time_refresh_type() == time_refresh_type &&
                              element.get_parameter() == parameter)
                          {
                              element.remove_component(player_component);
                              return element.get_component().empty();
                          }
                          return false;
                      }) > 0)
    {
        const auto old_next_refresh_time = get_next_refresh_time();
        calculate_next_refresh_time();
        if (old_next_refresh_time != get_next_refresh_time())
        {
            if (player_timer_ != nullptr)
            {
                player_timer_->stop();
                player_timer_->wait_for_next_tick();
            }
            else
            {
                init_player_timer(time_helper::get_current_milliseconds());
            }
        }
    }
}

int64_t celeritas::player_time_component::get_next_refresh_time() const
{
    return next_refresh_time_;
}

void celeritas::player_time_component::calculate_next_refresh_time()
{
    next_refresh_time_ = 0LL;
    const auto current_milliseconds = time_helper::get_current_milliseconds();
    for (auto& element : player_time_refresh_)
    {
        if (element.is_default())
        {
            continue;
        }
        if (const auto next_refresh_time = element.get_next_refresh_time();
            next_refresh_time > current_milliseconds &&
            (next_refresh_time_ == 0 || next_refresh_time_ > next_refresh_time))
        {
            next_refresh_time_ = next_refresh_time;
        }
    }
}

celeritas::player_component::void_awaitable_type celeritas::player_time_component::do_time_callback()
{
    const auto current_milliseconds = time_helper::get_current_milliseconds();
    for (auto& element : player_time_refresh_)
    {
        if (element.is_default())
        {
            continue;
        }

        if (element.is_can_refresh())
        {
            for (const auto& component : element.get_component())
            {
                co_await get_player_state()->get_component(component)->time_callback(element.get_time_refresh_type(), element.get_parameter(), false);
            }

            element.set_last_refresh_time(current_milliseconds);
        }
    }
    co_return;
}

void celeritas::player_time_component::wait_for_next_tick()
{
    if (next_refresh_time_ > 0)
    {
        const auto current_milliseconds = time_helper::get_current_milliseconds();
        if (player_timer_ == nullptr)
        {
            init_player_timer(current_milliseconds);
        }
        else
        {
            const std::chrono::milliseconds duration{ next_refresh_time_ - current_milliseconds };
            player_timer_->set_duration_type(duration);
            player_timer_->wait_for_next_tick();
        }
    }
}
