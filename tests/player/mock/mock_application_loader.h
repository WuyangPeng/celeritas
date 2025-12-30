#pragma once

#include "common/framework/application_loader_base.h"

namespace celeritas
{
    class mock_application_loader final : public application_loader_base
    {
    public:
        void submit_task(task_type task) override;
    };
}
