#include "change_timer_result.h"
#include "player_timer.h"
#include "player_time_scheduler.h"
#include "common/core/noexcept_safe_call_and_log.h"
#include "common/core/time_helper.h"
#include "common/logging/logger.h"
#include "player/component/player_state.h"
#include "player/time/player_time_component.h"

#include <boost/polymorphic_pointer_cast.hpp>

celeritas::player_time_scheduler::player_time_scheduler(player_state* player_state, player_time_component* time_component, player_time_document* player_time_document)
    : player_state_{ player_state }, time_component_{ time_component }, player_time_document_{ player_time_document }, next_refresh_time_{}, player_timer_{}
{
}

void celeritas::player_time_scheduler::register_timer(const player_component_type component_type, const player_time_refresh_key& player_time_refresh_key, const function_type& on_change)
{
    const auto result = player_time_document_->register_timer(component_type, player_time_refresh_key);

    if (result != change_timer_result::no_change)
    {
        on_change();
    }

    if (result == change_timer_result::change_document)
    {
        change_player_timer();
    }
}

void celeritas::player_time_scheduler::remove_timer(const player_component_type component_type, const player_time_refresh_key& player_time_refresh_key, const function_type& on_change)
{
    const auto result = player_time_document_->remove_timer(component_type, player_time_refresh_key);
    if (result != change_timer_result::no_change)
    {
        on_change();
    }

    if (result == change_timer_result::change_document)
    {
        change_player_timer();
    }
}

celeritas::player_time_scheduler::void_awaitable_type celeritas::player_time_scheduler::on_time_callback(const function_type& on_change)
{
    co_await noexcept_safe_call_and_log_awaitable([on_change = on_change,self = shared_from_this()] {
                                                      return self->do_on_time_callback(on_change);
                                                  },
                                                  player_channel,
                                                  "on time callback error:");
}

celeritas::player_time_scheduler::void_awaitable_type celeritas::player_time_scheduler::on_time_callback(const player_time_refresh_key& player_time_refresh_key, const bool is_login, const function_type& on_change)
{
    if (const auto result = co_await player_time_document_->on_time_callback(player_time_refresh_key, is_login);
        result != change_timer_result::no_change)
    {
        on_change();

        calculate_next_refresh_time();
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
        player_timer_ = std::make_unique<player_timer>(player_state_->get_any_io_executor(), duration, boost::polymorphic_pointer_cast<player_time_component>(time_component_->shared_from_this()));

        player_timer_->start();
    }
}

int64_t celeritas::player_time_scheduler::get_next_refresh_time() const
{
    return next_refresh_time_;
}

void celeritas::player_time_scheduler::calculate_next_refresh_time()
{
    next_refresh_time_ = player_time_document_->calculate_next_refresh_time();
}

void celeritas::player_time_scheduler::stop_timer() const
{
    if (player_timer_ != nullptr)
    {
        player_timer_->stop();
    }
}

celeritas::player_time_scheduler::void_awaitable_type celeritas::player_time_scheduler::do_time_callback(const function_type& on_change)
{
    if (const auto result = co_await player_time_document_->on_time_callback(false, false);
        result != change_timer_result::no_change)
    {
        on_change();

        calculate_next_refresh_time();
    }
}

void celeritas::player_time_scheduler::change_player_timer()
{
    const auto old_next_refresh_time = next_refresh_time_;
    next_refresh_time_ = player_time_document_->calculate_next_refresh_time();
    if (old_next_refresh_time != next_refresh_time_)
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

celeritas::player_time_scheduler::void_awaitable_type celeritas::player_time_scheduler::do_on_time_callback(const function_type& on_change)
{
    co_await do_time_callback(on_change);

    next_refresh_time_ = player_time_document_->calculate_next_refresh_time();
    wait_for_next_tick();
}