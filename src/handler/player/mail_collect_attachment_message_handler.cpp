#include "mail_collect_attachment_message_handler.h"
#include "handler/player/manual/player_concrete_message_handler.tpp"
#include "player/mail/mail_collect_attachment.h"

bool celeritas::mail_collect_attachment_message_handler::handle_concrete(const protobuf_handle_parameter_shared_ptr& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    player_co_spawn_response<mail_collect_attachment>(handle_parameter, current_message, handler_channel, "mail collect attachment error:");

    return true;
}