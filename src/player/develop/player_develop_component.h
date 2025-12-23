#pragma once

#include "detail/player_develop_database.h"
#include "detail/player_develop_document.h"
#include "message/message_fwd.h"
#include "player/component/player_component.h"
#include "player/component/player_component_type.h"

namespace celeritas
{
    class player_develop_component final : public player_component
    {
    public:
        using class_type = player_develop_component;
        using base_type = player_component;

        explicit player_develop_component(player_state* player_state) noexcept;

        [[nodiscard]] static constexpr player_component_type get_player_component_type()
        {
            return player_component_type::develop;
        }

        [[nodiscard]] void_awaitable_type on_load_db() override;

        [[nodiscard]] void_awaitable_type save_db() override;

        [[nodiscard]] bool is_modify() const override;

        [[nodiscard]] game_error_type develop_level(const develop_data_key& key);

        [[nodiscard]] game_error_type develop_reset(const develop_data_key& key);

    private:
        void update_document();

        player_develop_database database_;
        player_develop_document document_;
    };
}

