#pragma once

namespace celeritas
{
    enum class task_status_type
    {
        in_progress = 0,
        claimable = 1,
        completed = 2,
    };
}
