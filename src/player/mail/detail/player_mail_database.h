#pragma once

#include "database/generated/mongo/admin/server_mail.h"
#include "database/generated/mongo/player/user_mail.h"
#include "player/player_fwd.h"

#include <boost/asio/awaitable.hpp>

#include <map>
#include <optional>
#include <set>

namespace celeritas
{
    class player_mail_database
    {
    public:
        using class_type = player_mail_database;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using user_mail_shared_ptr = std::shared_ptr<user_mail>;
        using mail_container = std::map<int64_t, user_mail_shared_ptr, std::greater<> >;
        using optional_user_mail = std::optional<user_mail_shared_ptr>;
        using delete_mail_container = std::set<user_mail_shared_ptr>;
        using const_app_config_shared_ptr = std::shared_ptr<const app_config>;
        using const_server_mail_shared_ptr = std::shared_ptr<const server_mail>;

        player_mail_database(player_state* player_state, player_mail_component* player_mail_component);

        [[nodiscard]] void_awaitable_type load_user_mail();

        void load_server_mail(const const_app_config_shared_ptr& app_config);

        [[nodiscard]] mail_container& get_mail_data();

        [[nodiscard]] const mail_container& get_mail_data() const;

        [[nodiscard]] optional_user_mail get_mail(int64_t mail_id);

        [[nodiscard]] void_awaitable_type save_db();

        [[nodiscard]] bool is_modify() const;

        [[nodiscard]] void_awaitable_type delete_mail(const user_mail_shared_ptr& mail);

        [[nodiscard]] void_awaitable_type delete_mail(const delete_mail_container& mail);

        [[nodiscard]] void_awaitable_type add_server_mail(const const_app_config_shared_ptr& app_config, const const_server_mail_shared_ptr& server_mail);

    private:
        player_state* player_state_;
        player_mail_component* player_mail_component_;
        mail_container mail_container_;
        int64_t max_server_mail_id_;
    };
}
