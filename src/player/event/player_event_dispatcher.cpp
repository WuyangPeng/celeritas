#include "player_event_dispatcher.h"

#include <algorithm>
#include <ranges>

int64_t celeritas::player_event_dispatcher::register_listener(const player_event_type event_type, const player_event_listener_shared_ptr& listener)
{
    const auto id = next_listener_id_++;
    auto& listeners = listeners_[event_type];
    listeners.emplace_back(id, listener);

    // 按优先级排序
    std::ranges::sort(listeners,
                      [](const auto& lhs, const auto& rhs) {
                          return lhs.second->get_priority() < rhs.second->get_priority();
                      });

    return id;
}

void celeritas::player_event_dispatcher::unregister_listener(const player_event_type event_type, const int64_t listener_id)
{
    if (const auto iter = listeners_.find(event_type);
        iter != listeners_.cend())
    {
        auto& element = iter->second;
        std::erase_if(element,
                      [listener_id](const auto& pair) {
                          return pair.first == listener_id;
                      });
    }
}

celeritas::player_event_dispatcher::void_awaitable_type celeritas::player_event_dispatcher::dispatch(const player_event_shared_ptr& event)
{
    if (const auto iter = listeners_.find(event->get_type());
        iter != listeners_.cend())
    {
        for (const auto& listener : iter->second | std::views::values)
        {
            co_await listener->on_event(event);
        }
    }
}

void celeritas::player_event_dispatcher::clear()
{
    listeners_.clear();
}
