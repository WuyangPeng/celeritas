#include "item_container.h"
#include "player_item_component.h"
#include "config/game/game_config.h"
#include "config/game/game_tables.h"
#include "log/helper/item_flow_log_param.h"
#include "log/helper/item_log_helper.h"
#include "log/helper/log_param.h"

celeritas::player_item_component::player_item_component(player_state* player_state) noexcept
    : base_type{ get_player_component_type(), player_state },
      database_{ player_state, this },
      document_{ player_state },
      selected_database_{ player_state, this },
      selected_document_{ player_state },
      resource_{ &document_ }
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
    const int64_t before_count = get_count(template_id);
    if (co_await document_.change_item(app_config, template_id, count, is_login))
    {
        update_document();
        const int64_t after_count = get_count(template_id);
        log_item_flow("produce", template_id, count, before_count, after_count);
    }
}

celeritas::player_component::void_awaitable_type celeritas::player_item_component::consume_item(const const_app_config_shared_ptr& app_config, const int template_id, const int64_t count)
{
    const int64_t before_count = get_count(template_id);
    if (co_await document_.change_item(app_config, template_id, -count, false))
    {
        update_document();
        const int64_t after_count = get_count(template_id);
        log_item_flow("consume", template_id, -count, before_count, after_count);
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
    std::vector<std::pair<int, int64_t>> before_counts;
    for (const auto& element : item)
    {
        before_counts.emplace_back(element.get_template_id(), get_count(element.get_template_id()));
    }

    if (co_await document_.change_item(app_config, item, is_login))
    {
        update_document();
        for (const auto& [template_id, before_count] : before_counts)
        {
            const int64_t after_count = get_count(template_id);
            int64_t count_change = 0;
            for (const auto& element : item)
            {
                if (element.get_template_id() == template_id)
                {
                    count_change += element.get_count();
                }
            }
            log_item_flow("produce", template_id, count_change, before_count, after_count);
        }
    }
}

celeritas::player_component::void_awaitable_type celeritas::player_item_component::consume_item(const const_app_config_shared_ptr& app_config, const item_container& item)
{
    std::vector<std::pair<int, int64_t>> before_counts;
    for (const auto& element : item)
    {
        before_counts.emplace_back(element.get_template_id(), get_count(element.get_template_id()));
    }

    if (co_await document_.change_item(app_config, item.to_consume(), false))
    {
        update_document();
        for (const auto& [template_id, before_count] : before_counts)
        {
            const int64_t after_count = get_count(template_id);
            int64_t count_change = 0;
            for (const auto& element : item)
            {
                if (element.get_template_id() == template_id)
                {
                    count_change -= element.get_count();
                }
            }
            log_item_flow("consume", template_id, count_change, before_count, after_count);
        }
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

bool celeritas::player_item_component::lock_item(const int64_t item_id, const bool is_locked)
{
    if (const auto optional_iter = document_.get_inventory_data(item_id);
        optional_iter)
    {
        auto& inventory = optional_iter.value()->second;
        inventory.set_is_locked(is_locked);
        update_document();

        player_item_document::inventory_data_container container{ { inventory.get_item_id(), inventory } };
        document_.send_item_message(false, container);
        return true;
    }
    return false;
}

void celeritas::player_item_component::update_document()
{
    database_.set_item(document_.get_item());
}

celeritas::item_flow_log_param celeritas::player_item_component::generate_item_flow_log_param(const std::string& action,
                                                                                              int64_t item_id,
                                                                                              int32_t template_id,
                                                                                              int64_t count_change,
                                                                                              int64_t before_count,
                                                                                              int64_t after_count,
                                                                                              const std::string& source_id) const
{
    return item_flow_log_param{ action, item_id, template_id, count_change, before_count, after_count, source_id };
}

void celeritas::player_item_component::log_item_flow(const std::string& action, const int template_id, const int64_t count_change, const int64_t before_count, const int64_t after_count)
{
    const auto log_param = get_player_state()->generate_log_param();
    const auto item_param = generate_item_flow_log_param(action, 0, template_id, count_change, before_count, after_count, "");
    item_log_helper::add_item_flow_log(log_param, item_param);
}

