#include "player_mail_component.h"
#include "common/core/enum_cast.h"
#include "common/logging/logger.h"
#include "database/document/inventory_data.h"
#include "database/document/language_data.h"
#include "database/document/language_type.h"
#include "database/generated/mongo/player/user_mail.h"
#include "initializer/initializer_constant.h"
#include "player/component/player_state.h"
#include "player/item/detail/player_item_document.h"
#include "player/mail/detail/mail_status.h"
#include "proto/celeritas.pb.h"

celeritas::player_mail_component::player_mail_component(player_state* player_state) noexcept
    : base_type{ get_player_component_type(), player_state },
      database_{ player_state, this }
{
}

celeritas::player_component::void_awaitable_type celeritas::player_mail_component::on_load_db()
{
    co_return co_await database_.load_user_mail();
}

celeritas::player_component::void_awaitable_type celeritas::player_mail_component::save_db()
{
    co_return co_await database_.save_db();
}

bool celeritas::player_mail_component::is_modify() const
{
    return database_.is_modify();
}

celeritas::player_component::void_awaitable_type celeritas::player_mail_component::sync_mail(int rpc, const int64_t max_mail_id, const language_type language_type)
{
    const header header{ rpc, get_player_state()->get_user_id() };

    proto::celeritas response{};
    auto* mail_response = response.mutable_celeritas_response()->mutable_client()->mutable_player()->mutable_mail()->mutable_mail();

    for (const auto& mail_container = database_.get_mail_data();
         const auto& [mail_id, mail] : mail_container)
    {
        if (mail_id <= max_mail_id)
        {
            co_return;
        }

        auto* mail_proto = mail_response->add_mail();
        mail_proto->set_mail_id(mail->get_id());
        mail_proto->set_type(mail->get_type());

        const auto title_document = mail->get_title();
        const auto title_data = language_data::from_document(title_document);
        mail_proto->set_title(title_data.get_language_text(language_type));

        const auto content_document = mail->get_content();
        const auto content_data = language_data::from_document(content_document);
        mail_proto->set_content(content_data.get_language_text(language_type));

        mail_proto->set_send_time(mail->get_send_time());
        mail_proto->set_expire_time(mail->get_expire_time());

        const auto status = mail->get_status();
        mail_proto->set_read(status != enum_cast_underlying(mail_status::unread));
        mail_proto->set_attachment_collected(status == enum_cast_underlying(mail_status::attachment_collected));

        for (const auto attachments = mail->get_attachments();
             const auto& attachment : attachments)
        {
            const auto inventory_data = inventory_data::from_document(attachment);
            auto* inventory_proto = mail_proto->add_attachments();
            player_item_document::set_inventory_data_proto(inventory_proto, inventory_data);
        }
    }

    if (!get_player_state()->write(gateway_type.data(), get_player_state()->get_instance_id(), header, response))
    {
        LOG_CHANNEL(player_channel, error) << "send message error.";
    }

    co_return;
}

celeritas::game_error_type celeritas::player_mail_component::read_mail(const int64_t mail_id)
{
    const auto optional_mail = database_.get_mail(mail_id);
    if (!optional_mail)
    {
        return game_error_type::mail_not_exist;
    }

    const auto& mail = *optional_mail;

    if (const auto status = mail->get_status();
        status == enum_cast_underlying(mail_status::unread))
    {
        if (const auto attachments = mail->get_attachments();
            attachments.empty())
        {
            mail->set_status(enum_cast_underlying(mail_status::attachment_collected));
        }
        else
        {
            mail->set_status(enum_cast_underlying(mail_status::read));
        }
    }

    return game_error_type::success;
}

celeritas::game_error_type celeritas::player_mail_component::collect_mail_attachment(const int64_t mail_id)
{
    const auto optional_mail = database_.get_mail(mail_id);
    if (!optional_mail)
    {
        return game_error_type::mail_not_exist;
    }

    const auto& mail = *optional_mail;
    if (const auto status = mail->get_status();
        status == enum_cast_underlying(mail_status::attachment_collected))
    {
        return game_error_type::mail_attachment_collected;
    }

    if (const auto attachments = mail->get_attachments();
        attachments.empty())
    {
        return game_error_type::mail_no_attachment;
    }

    optional_mail.value()->set_status(enum_cast_underlying(mail_status::attachment_collected));
    return game_error_type::success;
}

celeritas::player_mail_component::game_error_type_awaitable_type celeritas::player_mail_component::delete_mail(const int64_t mail_id)
{
    const auto optional_mail = database_.get_mail(mail_id);
    if (!optional_mail)
    {
        co_return game_error_type::mail_not_exist;
    }

    const auto& mail = *optional_mail;

    const auto status = mail->get_status();
    if (status == enum_cast_underlying(mail_status::unread))
    {
        co_return game_error_type::mail_not_read;
    }

    if (const auto attachments = mail->get_attachments();
        !attachments.empty() && status != enum_cast_underlying(mail_status::attachment_collected))
    {
        co_return game_error_type::mail_attachment_not_collected;
    }

    co_await database_.delete_mail(mail);

    co_return game_error_type::success;
}

celeritas::game_error_type celeritas::player_mail_component::collect_all_mail_attachments()
{
    for (auto& mail_container = database_.get_mail_data();
         auto& mail : mail_container | std::views::values)
    {
        const auto status = mail->get_status();

        if (const auto attachments = mail->get_attachments();
            !attachments.empty() && status != enum_cast_underlying(mail_status::attachment_collected))
        {
            mail->set_status(enum_cast_underlying(mail_status::attachment_collected));
        }
    }

    return game_error_type::success;
}

celeritas::player_mail_component::game_error_type_awaitable_type celeritas::player_mail_component::delete_all_read_mails()
{
    player_mail_database::delete_mail_container container{};

    for (const auto& mail_container = database_.get_mail_data();
         const auto& element : mail_container | std::views::values)
    {
        if (const auto status = element->get_status();
            status != enum_cast_underlying(mail_status::unread))
        {
            if (const auto attachments = element->get_attachments();
                attachments.empty() || status == enum_cast_underlying(mail_status::attachment_collected))
            {
                container.emplace(element);
            }
        }
    }

    co_await database_.delete_mail(container);

    co_return game_error_type::success;
}
