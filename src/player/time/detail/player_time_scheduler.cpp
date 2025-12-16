#include "player_time_scheduler.h"
#include "common/logger.h"
#include "common/time_helper.h"
#include "player_timer.h"
#include "player/component/player_state.h"
#include "player/time/player_time_component.h"

#include <boost/polymorphic_pointer_cast.hpp>

celeritas::player_time_scheduler::player_time_scheduler(player_state* player_state, player_time_component* time_component)
    : player_state_{ player_state }, time_component_{ time_component }, next_refresh_time_{}, player_timer_{}
{
}

void celeritas::player_time_scheduler::register_timer(player_time_refresh_container& container, const player_component_type component_type, const time_refresh_type refresh_type, const int64_t parameter, const function_type& on_change)
{
    if (const auto iter = container.find(player_time_refresh_key{ refresh_type, parameter });
        iter != container.end())
    {
        auto& element = iter->second;
        const auto component = element.get_component();
        if (const auto component_iter = std::ranges::find(component, component_type);
            component_iter != component.cend())
        {
            return;
        }

        element.add_component(component_type);
        on_change();
        return;
    }

    container.emplace(std::piecewise_construct,
                      std::forward_as_tuple(refresh_type, parameter),
                      std::forward_as_tuple(refresh_type, parameter, component_type));

    on_change();

    const auto old_next_refresh_time = get_next_refresh_time();
    next_refresh_time_ = time_component_->calculate_next_refresh_time();
    if (old_next_refresh_time != get_next_refresh_time())
    {
        if (player_timer_ != nullptr)
        {
            player_timer_->stop();
            player_timer_->wait_for_next_tick();
        }
        else
        {
            init_player_timer();
        }
    }
}

void celeritas::player_time_scheduler::remove_timer(player_time_refresh_container& container, const player_component_type component_type, const time_refresh_type refresh_type, const int64_t parameter, const function_type& on_change)
{
    if (const auto iter = container.find(player_time_refresh_key{ refresh_type, parameter });
        iter != container.end())
    {
        auto& element = iter->second;
        element.remove_component(component_type);
        if (element.get_component().empty())
        {
            container.erase(iter);
            const auto old_next_refresh_time = get_next_refresh_time();

            next_refresh_time_ = time_component_->calculate_next_refresh_time();
            if (old_next_refresh_time != get_next_refresh_time())
            {
                if (player_timer_ != nullptr)
                {
                    player_timer_->stop();
                    player_timer_->wait_for_next_tick();
                }
                else
                {
                    init_player_timer();
                }
            }
        }

        on_change();
    }
}

celeritas::player_time_scheduler::void_awaitable_type celeritas::player_time_scheduler::on_time_callback(player_time_refresh_container& container, const function_type& on_change)
{
    try
    {
        co_await do_time_callback(container, on_change);

        next_refresh_time_ = time_component_->calculate_next_refresh_time();
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
}

celeritas::player_time_scheduler::void_awaitable_type celeritas::player_time_scheduler::on_time_callback(player_time_refresh_container& container, const time_refresh_type refresh_type, const int64_t parameter, const bool is_login, const function_type& on_change)
{
    const auto current_milliseconds = time_helper::get_current_milliseconds();
    if (const auto iter = container.find(player_time_refresh_key{ refresh_type, parameter });
        iter != container.end())
    {
        if (auto& element = iter->second;
            element.is_can_refresh())
        {
            for (const auto& component : element.get_component())
            {
                co_await player_state_->get_component(component)->time_callback(element.get_time_refresh_type(), element.get_parameter(), is_login);
            }

            element.set_last_refresh_time(current_milliseconds);
        }

        on_change();
    }

    next_refresh_time_ = time_component_->calculate_next_refresh_time();
}

celeritas::player_time_scheduler::void_awaitable_type celeritas::player_time_scheduler::do_time_callback(player_time_refresh_container& container, const function_type& on_change)
{
    const auto current_milliseconds = time_helper::get_current_milliseconds();
    auto change = false;
    for (auto& element : container | std::views::values)
    {
        if (element.is_default())
        {
            continue;
        }

        if (element.is_can_refresh())
        {
            for (const auto& component : element.get_component())
            {
                co_await player_state_->get_component(component)->time_callback(element.get_time_refresh_type(), element.get_parameter(), false);
            }

            element.set_last_refresh_time(current_milliseconds);

            change = true;
        }
    }

    if (change)
    {
        on_change();
    }
}

void celeritas::player_time_scheduler::wait_for_next_tick()
{
    if (next_refresh_time_ > 0)
    {
        const auto current_milliseconds = time_helper::get_current_milliseconds();
        if (player_timer_ == nullptr)
        {
            init_player_timer();
        }
        else
        {
            const std::chrono::milliseconds duration{ next_refresh_time_ - current_milliseconds };
            player_timer_->set_duration_type(duration);
            player_timer_->wait_for_next_tick();
        }
    }
}

void celeritas::player_time_scheduler::init_player_timer()
{
    if (next_refresh_time_ > 0)
    {
        const auto current_milliseconds = time_helper::get_current_milliseconds();
        std::chrono::milliseconds duration{ next_refresh_time_ - current_milliseconds };
        player_timer_ = std::make_unique<player_timer>(player_state_->get_io_context(), duration, boost::polymorphic_pointer_cast<player_time_component>(time_component_->shared_from_this()));

        player_timer_->start();
    }
}

int64_t celeritas::player_time_scheduler::get_next_refresh_time() const
{
    return next_refresh_time_;
}
