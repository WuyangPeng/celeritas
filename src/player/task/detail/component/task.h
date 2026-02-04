#pragma once

#include "task_component_type.h"
#include "player/player_fwd.h"

#include <memory>

namespace celeritas
{
    class task
    {
    public:
        using class_type = task;
        using task_type = task_component_type;
        using task_shared_ptr = std::shared_ptr<class_type>;

        explicit task(player_state* player_state) noexcept;

        virtual ~task() noexcept = default;

        task(const task& rhs) = default;

        task& operator=(const task& rhs) = default;

        task(task&& rhs) noexcept = default;

        task& operator=(task&& rhs) noexcept = default;

        [[nodiscard]] virtual task_type get_task_component_type() const = 0;

    protected:
        player_state* player_state_;
    };
}
