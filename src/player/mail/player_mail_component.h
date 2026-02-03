#pragma once

#include "database/document/database_document_fwd.h"
#include "database/generated/mongo/admin/server_mail.h"
#include "detail/player_mail_database.h"
#include "message/message_fwd.h"
#include "player/component/player_component.h"
#include "player/component/player_component_type.h"

namespace celeritas
{
    class player_mail_component final : public player_component
    {
    public:
        using class_type = player_mail_component;
        using base_type = player_component;
        using game_error_type_awaitable_type = boost::asio::awaitable<game_error_type>;
        using const_server_mail_shared_ptr = std::shared_ptr<const server_mail>;
        using mail_id_container = std::vector<int64_t>;
        using mail_id_container_awaitable_type = boost::asio::awaitable<mail_id_container>;

        explicit player_mail_component(player_state* player_state) noexcept;

        [[nodiscard]] static constexpr player_component_type get_player_component_type()
        {
            return player_component_type::mail;
        }

        [[nodiscard]] void_awaitable_type on_load_db() override;

        [[nodiscard]] void_awaitable_type on_db_analysis(const const_app_config_shared_ptr& app_config) override;

        [[nodiscard]] void_awaitable_type save_db() override;

        [[nodiscard]] bool is_modify() const override;

        [[nodiscard]] void_awaitable_type sync_mail(int rpc, int64_t max_mail_id, language_type language_type);

        [[nodiscard]] game_error_type read_mail(int64_t mail_id);

        [[nodiscard]] game_error_type collect_mail_attachment(int64_t mail_id);

        [[nodiscard]] game_error_type_awaitable_type delete_mail(int64_t mail_id);

        [[nodiscard]] mail_id_container collect_all_mail_attachments();

        [[nodiscard]] mail_id_container_awaitable_type delete_all_read_mails();

        [[nodiscard]] void_awaitable_type add_server_mail(const const_app_config_shared_ptr& app_config, const const_server_mail_shared_ptr& server_mail);

    private:
        player_mail_database database_;
    };
}
