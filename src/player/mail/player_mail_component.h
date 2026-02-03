#pragma once

#include "database/document/database_document_fwd.h"
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

        explicit player_mail_component(player_state* player_state) noexcept;

        [[nodiscard]] static constexpr player_component_type get_player_component_type()
        {
            return player_component_type::mail;
        }

        [[nodiscard]] void_awaitable_type on_load_db() override;

        [[nodiscard]] void_awaitable_type save_db() override;

        [[nodiscard]] bool is_modify() const override;

        [[nodiscard]] void_awaitable_type sync_mail(int rpc, int64_t max_mail_id, language_type language_type);

        [[nodiscard]] game_error_type read_mail(int64_t mail_id);

        [[nodiscard]] game_error_type collect_mail_attachment(int64_t mail_id);

        [[nodiscard]] game_error_type_awaitable_type delete_mail(int64_t mail_id);

        [[nodiscard]] game_error_type collect_all_mail_attachments();

        [[nodiscard]] game_error_type_awaitable_type delete_all_read_mails();

    private:
        player_mail_database database_;
    };
}
