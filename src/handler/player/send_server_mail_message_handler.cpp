#include "send_server_mail_message_handler.h"
#include "config/basic/database_type.h"
#include "message/parameters/protobuf_handle_parameter.h"
#include "player/server_mail/server_mail_manager.h"

bool celeritas::send_server_mail_message_handler::handle_concrete(const protobuf_handle_parameter_shared_ptr& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    const auto& mail = current_message.mail();
    auto server_mail_shared_ptr = std::make_shared<server_mail>(database_type::mongo, mail.mail_id());
    server_mail_shared_ptr->set_type(mail.type());
    server_mail_shared_ptr->set_multilingual(mail.multilingual());

    server_mail_manager::get_instance().add_mail(handle_parameter->get_app_config(), server_mail_shared_ptr);
    return true;
}