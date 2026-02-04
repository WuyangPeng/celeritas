#pragma once

#include "player/task/detail/player_task_internal_fwd.h"

#include <cstdint>

namespace celeritas
{
    class task_progress
    {
    public:
        using class_type = task_progress;

        task_progress(int64_t id, int cfg_id);

        task_progress(int64_t id, int cfg_id, int progress);

        [[nodiscard]] int64_t get_id() const;

        [[nodiscard]] int get_cfg_id() const;

        [[nodiscard]] int get_progress() const;

        [[nodiscard]] task_status_type get_status() const;

        void set_progress(int progress);

        void add_progress(int progress);

        void set_status(task_status_type status);

    private:
        int64_t id_;
        int cfg_id_;
        int progress_;
        task_status_type status_;
    };
}
