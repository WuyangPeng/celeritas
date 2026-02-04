#pragma once

#include "common/core/enum_cast.h"
#include "detail/component/task.h"
#include "player/component/player_component.h"
#include "player/component/player_component_type.h"

namespace celeritas
{
    class player_task_component final : public player_component
    {
    public:
        using class_type = player_task_component;
        using base_type = player_component;
        using task_shared_ptr = task::task_shared_ptr;
        using task_container = std::array<task_shared_ptr, enum_cast_underlying(config::task_component_type::max)>;

        explicit player_task_component(player_state* player_state) noexcept;

        [[nodiscard]] static constexpr player_component_type get_player_component_type()
        {
            return player_component_type::task;
        }

    private:
        task_container tasks_;
    };
}
