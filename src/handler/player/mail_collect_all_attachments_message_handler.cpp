#include "mail_collect_all_attachments_message_handler.h"
#include "handler/player/manual/player_concrete_message_handler.tpp"
#include "player/mail/mail_collect_all_attachments.h"

bool celeritas::mail_collect_all_attachments_message_handler::handle_concrete(const protobuf_handle_parameter_shared_ptr& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    player_co_spawn_response<mail_collect_all_attachments>(handle_parameter, current_message, handler_channel, "mail collect all attachments error:");

    return true;
}