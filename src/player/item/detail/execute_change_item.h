#pragma once

#include "player_item_document.h"
#include "config/aggregate/app_config.h"
#include "database/document/develop_data.h"
#include "database/document/inventory_data.h"
#include "player/item/item_container.h"

#include <map>
#include <vector>

namespace celeritas
{
    class execute_change_item
    {
    public:
        using class_type = execute_change_item;
        using const_app_config_shared_ptr = std::shared_ptr<const app_config>;

        execute_change_item(player_state* player_state, player_item_document* player_item_document, const_app_config_shared_ptr app_config, int template_id, int64_t count);

        execute_change_item(player_state* player_state, player_item_document* player_item_document, const_app_config_shared_ptr app_config, item_container item);

        void execute();

        void send_message();

        void execute_develop();

        void send_develop_message();

        [[nodiscard]] bool is_change() const noexcept;

    private:
        using inventory_data_container = std::map<int64_t, inventory_data>;
        using id_container = std::vector<int64_t>;
        using const_item_config_shared_ptr = std::shared_ptr<const config::game::item_config>;
        using develop_container = std::set<const_item_config_shared_ptr>;
        using develop_data_container = std::vector<develop_data>;

        [[nodiscard]] bool execute(int template_id, int64_t count);

        [[nodiscard]] int64_t add_to_existing_stacks(int template_id, int64_t count, int stacked);

        [[nodiscard]] int64_t remove_from_existing_stacks(int template_id, int64_t count);

        [[nodiscard]] int64_t add_new_item(int template_id,
                                           int64_t count,
                                           int stacked,
                                           bool squares,
                                           const server_config& server_config);

        player_state* player_state_;
        player_item_document* player_item_document_;
        const_app_config_shared_ptr app_config_;
        item_container item_;
        inventory_data_container inventory_data_;
        inventory_data_container delete_inventory_data_;
        develop_container develop_;
        bool change_;
        develop_data_container change_develop_;
    };
}