#pragma once

#include "task.h"

namespace celeritas
{
    class avatar_task final : public task
    {
    public:
        using class_type = avatar_task;
        using base_type = task;

        explicit avatar_task(player_state* player_state) noexcept;

        [[nodiscard]] task_component_type get_task_component_type() const override;

        [[nodiscard]] void_awaitable_type on_db_analysis(const const_app_config_shared_ptr& app_config) override;
    };
}
