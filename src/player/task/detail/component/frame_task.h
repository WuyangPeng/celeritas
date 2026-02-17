#pragma once

#include "task.h"

namespace celeritas
{
    class frame_task final : public task
    {
    public:
        using class_type = frame_task;
        using base_type = task;

        explicit frame_task(player_state* player_state) noexcept;

        [[nodiscard]] task_component_type get_task_component_type() const override;

        [[nodiscard]] void_awaitable_type on_db_analysis(const const_app_config_shared_ptr& app_config) override;
    };
}
