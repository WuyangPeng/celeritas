#pragma once

#include "config/luban/generated/schema.h"
#include "player/player_fwd.h"
#include "player/task/detail/player_task_internal_fwd.h"

#include <map>
#include <memory>
#include <set>

namespace celeritas
{
    class task
    {
    public:
        using class_type = task;
        using task_component_type = config::task_component_type;
        using task_shared_ptr = std::shared_ptr<class_type>;
        using task_progress_shared_ptr = std::shared_ptr<task_progress>;

        explicit task(player_state* player_state) noexcept;

        virtual ~task() noexcept = default;

        task(const task& rhs) = default;

        task& operator=(const task& rhs) = default;

        task(task&& rhs) noexcept = default;

        task& operator=(task&& rhs) noexcept = default;

        [[nodiscard]] virtual task_component_type get_task_component_type() const = 0;

        [[nodiscard]] static config::task_event_type get_task_event_type(int cfg_id);

        void add_task_progress(const task_progress_shared_ptr& task_progress);

        void update_task_progress(const task_context& task_context);

    protected:
        using task_progress_container = std::map<int64_t, task_progress_shared_ptr>;
        using event_container = std::map<config::task_event_type, std::set<int64_t> >;

        player_state* player_state_;
        task_progress_container task_progress_;
        event_container event_;
    };
}
