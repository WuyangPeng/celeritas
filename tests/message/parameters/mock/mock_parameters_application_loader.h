#pragma once

#include "common/framework/application_loader_base.h"

namespace celeritas
{
    class mock_parameters_application_loader final : public application_loader_base
    {
    public:
        using class_type = mock_parameters_application_loader;
        using base_type = application_loader_base;

        void submit_task(task_type task) override;

        [[nodiscard]] bool get_task_submitted() const;

    private:
        std::atomic_bool task_submitted_{ false };
    };
}
