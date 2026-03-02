#include "item_container.h"
#include "player_item_component.h"
#include "config/game/game_config.h"
#include "config/game/game_tables.h"

celeritas::player_item_component::player_item_component(player_state* player_state) noexcept
    : base_type{ get_player_component_type(), player_state },
      database_{ player_state, this },
      document_{ player_state },
      selected_database_{ player_state, this },
      selected_document_{ player_state }
{
}

celeritas::player_component::void_awaitable_type celeritas::player_item_component::on_load_db()
{
    co_await database_.load_user_item();

    document_.set_item(database_.get_inventory_data());

    co_await selected_database_.load_user_item();

    selected_document_.set_item_selected(selected_database_.get_item_selected_data());

    co_return;
}

celeritas::player_component::void_awaitable_type celeritas::player_item_component::on_db_analysis(const const_app_config_shared_ptr& app_config)
{
    if (get_player_state()->is_new_user())
    {
        item_container container{};
        std::vector<int> wear_container{};
        for (const auto& item_config : game_config::get_instance().get_game_tables()->get_tables()->default_item_config_container.getDataList())
        {
            for (const auto& element : item_config->playerItem)
            {
                container.add_item_info(element->itemId, element->itemCount);
                if (item_config->wear)
                {
                    wear_container.emplace_back(element->itemId);
                }
            }
        }

        co_await produce_item(app_config, container, true);

        if (!wear_container.empty())
        {
            const auto item_config_container = game_config::get_instance().get_game_tables()->get_tables()->item_config_container;
            for (const auto element : wear_container)
            {
                if (const auto item_document = document_.get_id_container(element);
                    item_document != nullptr && !item_document->empty())
                {
                    if (const auto item_config = item_config_container.get(element))
                    {
                        std::ignore = change_item_selected(app_config,
                                                           (*item_config)->itemType,
                                                           config::item_selected_child_type::none,
                                                           0,
                                                           0,
                                                           item_document->at(0));
                    }
                }
            }
        }
    }

    co_return;
}

celeritas::player_component::void_awaitable_type celeritas::player_item_component::save_db()
{
    co_await database_.save_db();

    co_await selected_database_.save_db();

    co_return;
}

bool celeritas::player_item_component::is_modify() const
{
    return database_.is_modify() || selected_database_.is_modify();
}

celeritas::player_component::void_awaitable_type celeritas::player_item_component::produce_item(const const_app_config_shared_ptr& app_config,
                                                                                                const int template_id,
                                                                                                const int64_t count,
                                                                                                const bool is_login)
{
    if (co_await document_.change_item(app_config, template_id, count, is_login))
    {
        update_document();
    }
}

celeritas::player_component::void_awaitable_type celeritas::player_item_component::consume_item(const const_app_config_shared_ptr& app_config, const int template_id, const int64_t count)
{
    if (co_await document_.change_item(app_config, template_id, -count, false))
    {
        update_document();
    }
}

bool celeritas::player_item_component::can_consume_item(const int template_id, const int64_t count) const
{
    return document_.can_consume_item(template_id, count);
}

int64_t celeritas::player_item_component::get_count(const int template_id) const
{
    return document_.get_count(template_id);
}

bool celeritas::player_item_component::can_consume_item(const item_container& item) const
{
    return document_.can_consume_item(item);
}

celeritas::player_component::void_awaitable_type celeritas::player_item_component::produce_item(const const_app_config_shared_ptr& app_config,
                                                                                                const item_container& item,
                                                                                                const bool is_login)
{
    if (co_await document_.change_item(app_config, item, is_login))
    {
        update_document();
    }
}

celeritas::player_component::void_awaitable_type celeritas::player_item_component::consume_item(const const_app_config_shared_ptr& app_config, const item_container& item)
{
    if (co_await document_.change_item(app_config, item.to_consume(), false))
    {
        update_document();
    }
}

celeritas::player_item_component::optional_item_selected_data celeritas::player_item_component::change_item_selected(const const_app_config_shared_ptr& app_config,
                                                                                                                     const config::item_type item_type,
                                                                                                                     const config::item_selected_child_type child_type,
                                                                                                                     const int64_t operation_id,
                                                                                                                     const int parameter,
                                                                                                                     const int64_t selected_id)
{
    const auto optional_selected_data = selected_document_.change_item_selected(app_config, item_type, child_type, operation_id, parameter, selected_id);
    if (optional_selected_data)
    {
        selected_database_.set_item(selected_document_.get_item_selected());
    }

    return optional_selected_data;
}

celeritas::player_component::void_awaitable_type celeritas::player_item_component::send_initial_sync()
{
    document_.send_initial_sync();
    selected_document_.send_initial_sync();

    co_return;
}

void celeritas::player_item_component::send_item_message(const bool is_login, const int rpc, const item_selected_data& item_selected)
{
    const item_selected_key item_selected_key{ item_selected.get_item_type(), item_selected.get_child_type(), item_selected.get_operation_id(), item_selected.get_parameter() };

    selected_document_.send_item_message(is_login, rpc, player_item_selected_document::item_selected_data_container{ { item_selected_key, item_selected } });
}

bool celeritas::player_item_component::has_item(const int64_t item_id) const
{
    return document_.has_item(item_id);
}

void celeritas::player_item_component::update_document()
{
    database_.set_item(document_.get_item());
}

