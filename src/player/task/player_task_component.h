#pragma once

#include "common/core/enum_cast.h"
#include "detail/component/task.h"
#include "detail/default_progress/default_progress_calculator.h"
#include "player/component/player_component.h"
#include "player/component/player_component_type.h"

#include <array>
#include <memory>
#include <unordered_map>

namespace celeritas
{
    class player_task_component final : public player_component
    {
    public:
        using class_type = player_task_component;
        using base_type = player_component;
        using task_shared_ptr = task::task_shared_ptr;
        using task_container = std::array<task_shared_ptr, enum_cast_underlying(config::task_component_type::max)>;
        using calculator_shared_ptr = std::shared_ptr<default_progress_calculator>;
        using calculator_container = std::unordered_map<config::task_event_type, calculator_shared_ptr>;

        explicit player_task_component(player_state* player_state) noexcept;

        [[nodiscard]] void_awaitable_type on_db_analysis(const const_app_config_shared_ptr& app_config) override;

        [[nodiscard]] int get_default_progress(config::task_event_type task_event_type, int target_id);

        [[nodiscard]] static constexpr player_component_type get_player_component_type()
        {
            return player_component_type::task;
        }

        void update_task_progress(const task_context& task_context, bool is_login);

    private:
        task_container tasks_;
        calculator_container calculators_;
    };
}
