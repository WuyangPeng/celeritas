#pragma once

namespace celeritas
{
    enum class task_component_type;

    class task;
    class main_task;
    class daily_task;

    enum class task_status_type;
    enum class task_change_type;
    class task_progress;
    class task_context;

    class task_condition;
    class kill_monster_condition;
    class upgrade_level_condition;
}