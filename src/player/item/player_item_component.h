#pragma once

#include "detail/player_item_database.h"
#include "detail/player_item_document.h"
#include "player/component/player_component.h"
#include "player/component/player_component_type.h"

namespace celeritas
{
    class player_item_component final : public player_component
    {
    public:
        using class_type = player_item_component;
        using base_type = player_component;

        explicit player_item_component(player_state* player_state) noexcept;

        [[nodiscard]] static constexpr player_component_type get_player_component_type()
        {
            return player_component_type::item;
        }

        [[nodiscard]] void_awaitable_type on_load_db() override;

        [[nodiscard]] void_awaitable_type save_db() override;

        [[nodiscard]] bool is_modify() const override;

        void change_item(const const_app_config_shared_ptr& app_config, int template_id, int64_t count);

        [[nodiscard]] bool can_consume_item(int template_id, int64_t count) const;

        [[nodiscard]] int64_t get_count(int template_id) const;

        [[nodiscard]] bool can_consume_item(const item_container& item) const;

        void change_item(const const_app_config_shared_ptr& app_config, const item_container& item);

    private:
        void update_document();

        player_item_database database_;
        player_item_document document_;
    };
}