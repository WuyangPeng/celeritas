#include "server_mail_check_timer.h"
#include "common/core/time_helper.h"
#include "player/server_mail/server_mail_manager.h"

celeritas::server_mail_check_timer::server_mail_check_timer(const any_io_executor& any_io_executor, duration_type interval)
    : base_type{ any_io_executor, interval }
{
}

void celeritas::server_mail_check_timer::execute_timer_task()
{
    server_mail_manager::get_instance().clean_expired_mails(time_helper::get_current_milliseconds());
}