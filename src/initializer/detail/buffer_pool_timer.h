#pragma once

#include "common/timer_base.h"
#include "initializer/initializer_fwd.h"

namespace celeritas
{
    class buffer_pool_timer final : public timer_base
    {
    public:
        using class_type = buffer_pool_timer;
        using base_type = timer_base;

        explicit buffer_pool_timer(io_context_type& io_context, duration_type interval);

    private:
        void execute_timer_task() override;
    };
}

