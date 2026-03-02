#pragma once

namespace celeritas
{
    class task;
    class main_task;
    class daily_task;
    class avatar_task;
    class title_task;

    enum class task_change_type;
    class task_context;

    class task_condition;
    class kill_monster_condition;
    class upgrade_level_condition;
    class hold_item_condition;

    class default_progress_calculator;
    class kill_monster_progress_calculator;
    class upgrade_level_progress_calculator;
    class hold_item_progress_calculator;
}