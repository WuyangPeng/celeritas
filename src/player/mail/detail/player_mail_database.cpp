#include "player_mail_database.h"
#include "common/logging/logger.h"
#include "config/basic/database_type.h"
#include "database/pool/database_pool_base.h"
#include "player/component/player_state.h"
#include "player/mail/player_mail_component.h"
#include "player/user/player_user_component.h"

celeritas::player_mail_database::player_mail_database(player_state* player_state, player_mail_component* player_mail_component)
    : player_state_{ player_state }, player_mail_component_{ player_mail_component }, mail_container_{}
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
    for (auto& element : result)
    {
        user_mail mail{ element };
        mail_container_.emplace(mail.get_id(), std::make_shared<user_mail>(mail));
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

