#include "player_mail_database.h"
#include "common/core/snowflake_generator.h"
#include "common/logging/logger.h"
#include "config/aggregate/app_config.h"
#include "config/basic/database_type.h"
#include "database/pool/database_pool_base.h"
#include "player/component/player_state.h"
#include "player/mail/player_mail_component.h"
#include "player/server_mail/server_mail_manager.h"
#include "player/user/player_user_component.h"

celeritas::player_mail_database::player_mail_database(player_state* player_state, player_mail_component* player_mail_component)
    : player_state_{ player_state }, player_mail_component_{ player_mail_component }, mail_container_{}, max_server_mail_id_{}
{
}

celeritas::player_mail_database::void_awaitable_type celeritas::player_mail_database::load_user_mail()
{
    const auto mongo_player_pool = player_mail_component_->get_mongo_player_database_pool();
    const auto player_user = player_state_->get_component<player_user_component>();
    const auto user_id = player_user->get_user_id();

    const auto result = co_await mongo_player_pool->select_all(user_mail::get_select(database_type::mongo, user_id),
                                                               user_mail::get_database_field_container());

    mail_container_.clear();
    max_server_mail_id_ = 0;

    for (auto& element : result)
    {
        user_mail mail{ element };
        mail_container_.emplace(mail.get_id(), std::make_shared<user_mail>(mail));
        if (mail.get_id() > max_server_mail_id_)
        {
            max_server_mail_id_ = mail.get_id();
        }
    }
}

void celeritas::player_mail_database::load_server_mail(const const_app_config_shared_ptr& app_config)
{
    const auto player_user = player_state_->get_component<player_user_component>();
    const auto user_id = player_user->get_user_id();
    const auto server_config = app_config->get_server_config();

    for (const auto new_server_mails = server_mail_manager::get_instance().get_mails_greater_than_id(max_server_mail_id_);
         const auto& server_mail : new_server_mails)
    {
        const auto server_mail_id = server_mail->get_id();

        const auto user_mail_id = snowflake_generator::get_instance().generate(server_config->get_datacenter_id(), server_config->get_worker_id());

        user_mail mail{ database_type::mongo, user_mail_id };
        mail.set_user_id(user_id);
        mail.set_server_mail_id(server_mail_id);
        mail.set_type(server_mail->get_type());
        mail.set_multilingual(server_mail->is_multilingual());
        mail.set_title(server_mail->get_title());
        mail.set_content(server_mail->get_content());
        mail.set_attachments(server_mail->get_attachments());
        mail.set_send_time(server_mail->get_send_time());
        mail.set_expire_time(server_mail->get_expire_time());

        mail_container_.emplace(user_mail_id, std::make_shared<user_mail>(mail));

        if (server_mail_id > max_server_mail_id_)
        {
            max_server_mail_id_ = server_mail_id;
        }
    }
}

celeritas::player_mail_database::mail_container& celeritas::player_mail_database::get_mail_data()
{
    return mail_container_;
}

const celeritas::player_mail_database::mail_container& celeritas::player_mail_database::get_mail_data() const
{
    return mail_container_;
}

celeritas::player_mail_database::optional_user_mail celeritas::player_mail_database::get_mail(const int64_t mail_id)
{
    if (const auto iter = mail_container_.find(mail_id);
        iter != mail_container_.cend())
    {
        return iter->second;
    }

    return std::nullopt;
}

celeritas::player_mail_database::void_awaitable_type celeritas::player_mail_database::save_db()
{
    const auto mongo_player_pool = player_mail_component_->get_mongo_player_database_pool();
    std::vector<user_mail_shared_ptr> changes{};

    for (auto& mail : mail_container_ | std::views::values)
    {
        if (mail->is_must_save())
        {
            changes.emplace_back(mail);
        }
    }

    for (const auto& element : changes)
    {
        if (co_await mongo_player_pool->execute_changes(element->get_modify()))
        {
            element->clear_modify();
        }
    }
}

bool celeritas::player_mail_database::is_modify() const
{
    return std::ranges::any_of(mail_container_,
                               [](const auto& pair) {
                                   return pair.second->is_must_save();
                               });
}

celeritas::player_mail_database::void_awaitable_type celeritas::player_mail_database::delete_mail(const user_mail_shared_ptr& mail)
{
    if (const auto mongo_player_pool = player_mail_component_->get_mongo_player_database_pool();
        co_await mongo_player_pool->execute_changes(mail->get_delete()))
    {
        mail_container_.erase(mail->get_id());
    }
    else
    {
        LOG_CHANNEL(player_channel, error) << "delete mail error.id = " << mail->get_id();
    }
}

celeritas::player_mail_database::void_awaitable_type celeritas::player_mail_database::delete_mail(const delete_mail_container& mail)
{
    for (const auto& element : mail)
    {
        co_await delete_mail(element);
    }
}

celeritas::player_mail_database::void_awaitable_type celeritas::player_mail_database::add_server_mail(const const_app_config_shared_ptr& app_config, const const_server_mail_shared_ptr& server_mail)
{
    const auto server_mail_id = server_mail->get_id();

    for (const auto& user_mail : mail_container_ | std::views::values)
    {
        if (user_mail->get_server_mail_id() == server_mail_id)
        {
            co_return;
        }
    }

    const auto user_id = player_state_->get_user_id();
    const auto server_config = app_config->get_server_config();

    const auto user_mail_id = snowflake_generator::get_instance().generate(server_config->get_datacenter_id(), server_config->get_worker_id());

    user_mail mail{ database_type::mongo, user_mail_id };
    mail.set_user_id(user_id);
    mail.set_server_mail_id(server_mail_id);
    mail.set_type(server_mail->get_type());
    mail.set_multilingual(server_mail->is_multilingual());
    mail.set_title(server_mail->get_title());
    mail.set_content(server_mail->get_content());
    mail.set_attachments(server_mail->get_attachments());
    mail.set_send_time(server_mail->get_send_time());
    mail.set_expire_time(server_mail->get_expire_time());

    mail_container_.emplace(user_mail_id, std::make_shared<user_mail>(mail));

    if (server_mail_id > max_server_mail_id_)
    {
        max_server_mail_id_ = server_mail_id;
    }
}

