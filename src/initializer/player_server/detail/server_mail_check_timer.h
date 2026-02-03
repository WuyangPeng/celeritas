#pragma once

#include "common/framework/timer_base.h"

namespace celeritas
{
    class server_mail_check_timer final : public timer_base
    {
    public:
        using class_type = server_mail_check_timer;
        using base_type = timer_base;

        explicit server_mail_check_timer(const any_io_executor& any_io_executor, duration_type interval);

    private:
        void execute_timer_task() override;
    };
}