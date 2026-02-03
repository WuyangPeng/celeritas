#include "send_server_mail_message_handler.h"
#include "common/core/enum_cast.h"
#include "config/basic/database_type.h"
#include "message/parameters/protobuf_handle_parameter.h"
#include "player/server_mail/server_mail_manager.h"
#include "database/document/language_data.h"
#include "database/document/inventory_data.h"
#include "database/document/language_type.h"
#include "player/item/detail/player_item_database.h"
#include "player/item/detail/player_item_document.h"
#include "proto/common/item.pb.h"
#include "proto/client/player/mail.pb.h"

bool celeritas::send_server_mail_message_handler::handle_concrete(const protobuf_handle_parameter_shared_ptr& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    const auto& mail = current_message.mail();
    auto server_mail_shared_ptr = std::make_shared<server_mail>(database_type::mongo, mail.mail_id());
    server_mail_shared_ptr->set_type(mail.type());
    server_mail_shared_ptr->set_multilingual(mail.multilingual());

    language_data title{};
    for (const auto& element : mail.title())
    {
        title.set_language_text(underlying_cast_enum<language_type>(element.type()), element.content());
    }
    server_mail_shared_ptr->set_title(title.to_document_type());

    language_data content{};
    for (const auto& element : mail.content())
    {
        title.set_language_text(underlying_cast_enum<language_type>(element.type()), element.content());
    }
    server_mail_shared_ptr->set_content(title.to_document_type());
    traits::document_array_type attachments_array{};
    for (const auto& attachment : mail.attachments())
    {
        attachments_array.emplace_back(player_item_document::get_inventory_data_by_proto(&attachment).to_document_type());
    }
    server_mail_shared_ptr->set_attachments(attachments_array);

    server_mail_shared_ptr->set_send_time(mail.send_time());
    server_mail_shared_ptr->set_expire_time(mail.expire_time());

    server_mail_manager::get_instance().add_mail(handle_parameter->get_app_config(), server_mail_shared_ptr);
    return true;
}