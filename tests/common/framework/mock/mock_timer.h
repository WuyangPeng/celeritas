#pragma once

#include "common/framework/timer_base.h"

namespace celeritas
{
    class mock_timer final : public timer_base
    {
    public:
        using class_type = mock_timer;
        using base_type = timer_base;

        mock_timer(const any_io_executor& any_io_executor, duration_type interval, bool disposable);

        void execute_timer_task() override;

        [[nodiscard]] int get_execution_count() const;

    private:
        std::atomic<int> execution_count{ 0 };
    };
}