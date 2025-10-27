#pragma once

#include "common/timer_base.h"
#include "database/database_fwd.h"

namespace celeritas
{
    class cleanup_database_session_timer final : public timer_base
    {
    public:
        using class_type = cleanup_database_session_timer;
        using base_type = timer_base;
        using database_pool_weak_ptr = std::weak_ptr<database_pool>;

        explicit cleanup_database_session_timer(io_context_type& io_context, duration_type interval, database_pool_weak_ptr database_pool);

    private:
        void execute_timer_task() override;

        database_pool_weak_ptr database_pool_;
    };
}