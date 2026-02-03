#include "server_mail_manager.h"
#include "common/core/noexcept_safe_call_and_log.h"
#include "common/core/time_helper.h"
#include "common/logging/logger.h"
#include "config/basic/database_type.h"
#include "database/pool/database_pool_base.h"
#include "database/pool/database_pool_manager.h"
#include "player/component/player_manager.h"

#include <ranges>

celeritas::server_mail_manager& celeritas::server_mail_manager::get_instance()
{
    static server_mail_manager instance{};
    return instance;
}

void celeritas::server_mail_manager::load_all_mails(const any_io_executor& any_io_executor)
{
    boost::asio::co_spawn(any_io_executor,
                          noexcept_safe_call_and_log_awaitable([ ] {
                                                                   return get_instance().load_all_mails();
                                                               },
                                                               auth_channel,
                                                               "load all mails error:"),

                          boost::asio::detached);
}

celeritas::server_mail_manager::optional_const_server_mail_shared_ptr celeritas::server_mail_manager::get_mail(const int64_t mail_id)
{
    std::shared_lock lock{ mutex_ };
    if (const auto iter = mails_.find(mail_id);
        iter != mails_.end())
    {
        return iter->second;
    }

    return std::nullopt;
}

celeritas::server_mail_manager::server_mail_container celeritas::server_mail_manager::get_mails_by_type(const int32_t mail_type)
{
    std::shared_lock lock{ mutex_ };

    server_mail_container result{};

    for (const auto& mail : mails_ | std::views::values)
    {
        if (mail->get_type() == mail_type)
        {
            result.emplace_back(mail);
        }
    }

    return result;
}

celeritas::server_mail_manager::server_mail_container celeritas::server_mail_manager::get_active_mails()
{
    std::shared_lock lock{ mutex_ };

    const auto current_time = time_helper::get_current_milliseconds();
    server_mail_container result{};

    for (const auto& mail : mails_ | std::views::values)
    {
        if (!is_expired(*mail, current_time))
        {
            result.emplace_back(mail);
        }
    }

    return result;
}

celeritas::server_mail_manager::server_mail_container celeritas::server_mail_manager::get_all_mails()
{
    std::shared_lock lock{ mutex_ };

    server_mail_container result{};
    result.reserve(mails_.size());

    for (const auto& mail : mails_ | std::views::values)
    {
        result.emplace_back(mail);
    }

    return result;
}

celeritas::server_mail_manager::server_mail_container celeritas::server_mail_manager::get_mails_greater_than_id(const int64_t mail_id)
{
    std::shared_lock lock{ mutex_ };

    const auto current_time = time_helper::get_current_milliseconds();
    server_mail_container result{};

    for (const auto& mail : mails_ | std::views::values)
    {
        if (mail->get_id() > mail_id && !is_expired(*mail, current_time))
        {
            result.emplace_back(mail);
        }
    }

    return result;
}

void celeritas::server_mail_manager::add_mail(const const_app_config_shared_ptr& app_config, const const_server_mail_shared_ptr& mail)
{
    {
        std::unique_lock lock{ mutex_ };
        mails_[mail->get_id()] = mail;
    }

    LOG_CHANNEL(player_channel, info) << "server_mail_manager added mail with id: " << mail->get_id();

    player_manager::get_instance().add_server_mail(app_config, mail);
}

void celeritas::server_mail_manager::remove_mail(const int64_t mail_id)
{
    std::unique_lock lock{ mutex_ };
    mails_.erase(mail_id);

    LOG_CHANNEL(player_channel, info) << "server_mail_manager removed mail with id: " << mail_id;
}

void celeritas::server_mail_manager::clean_expired_mails(const int64_t current_time)
{
    std::unique_lock lock{ mutex_ };

    for (auto iter = mails_.begin(); iter != mails_.end();)
    {
        if (is_expired(*iter->second, current_time))
        {
            iter = mails_.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}

void celeritas::server_mail_manager::clear()
{
    std::unique_lock lock{ mutex_ };
    mails_.clear();
    LOG_CHANNEL(player_channel, info) << "server_mail_manager cleared all mails";
}

celeritas::server_mail_manager::server_mail_manager()
{
    LOG_CHANNEL(player_channel, info) << "server mail manager initialized";
}

bool celeritas::server_mail_manager::is_expired(const server_mail& mail, const int64_t current_time)
{
    const auto expire_time = mail.get_expire_time();
    if (expire_time <= 0)
    {
        return false;
    }

    return current_time > expire_time;
}

celeritas::server_mail_manager::void_awaitable_type celeritas::server_mail_manager::load_all_mails()
{
    const auto mongo_pool = database_pool_manager::get_instance().get_pool(mongo_admin_db_name.data());

    const auto result = co_await mongo_pool->select_all(server_mail::get_select(database_type::mongo),
                                                        server_mail::get_database_field_container());

    container_type loaded_mails{};
    for (auto& element : result)
    {
        auto mail = std::make_shared<server_mail>(element);
        loaded_mails.emplace(mail->get_id(), std::move(mail));
    }

    std::unique_lock lock{ mutex_ };

    mails_ = std::move(loaded_mails);

    LOG_CHANNEL(player_channel, info) << "server mail manager loaded " << mails_.size() << " mails";
}