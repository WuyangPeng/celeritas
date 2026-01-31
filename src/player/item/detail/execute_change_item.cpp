#include "execute_change_item.h"
#include "common/core/snowflake_generator.h"
#include "common/logging/logger.h"

celeritas::execute_change_item::execute_change_item(player_item_document* player_item_document,
                                                    const_app_config_shared_ptr app_config,
                                                    const int template_id,
                                                    const int64_t count)
    : player_item_document_{ player_item_document },
      app_config_{ std::move(app_config) },
      item_{},
      inventory_data_{},
      id_{},
      change_{ false }
{
    item_.add_item_info(template_id, count);
}

celeritas::execute_change_item::execute_change_item(player_item_document* player_item_document,
                                                    const_app_config_shared_ptr app_config,
                                                    item_container item)
    : player_item_document_{ player_item_document },
      app_config_{ std::move(app_config) },
      item_{ std::move(item) },
      inventory_data_{},
      id_{},
      change_{ false }
{
}

void celeritas::execute_change_item::execute()
{
    for (const auto& element : item_)
    {
        if (execute(element.get_template_id(), element.get_count()))
        {
            change_ = true;
        }
    }
}

void celeritas::execute_change_item::send_message()
{
    if (!inventory_data_.empty())
    {
        player_item_document_->send_item_message(false, inventory_data_);
    }

    if (!id_.empty())
    {
        player_item_document_->send_delete_item_message(id_);
    }
}

bool celeritas::execute_change_item::is_change() const noexcept
{
    return change_;
}

bool celeritas::execute_change_item::execute(const int template_id, int64_t count)
{
    if (count == 0)
    {
        return false;
    }

    const auto item = player_item_document::get_item_config(template_id);
    const auto stacked = item->stacked;

    if (count > 0)
    {
        count = add_to_existing_stacks(template_id, count, stacked);
    }
    else
    {
        count = remove_from_existing_stacks(template_id, count);
    }

    if (count < 0)
    {
        LOG_CHANNEL(player_channel, error) << "item count is error ,template id = " << template_id << ",count = " << count;
        return true;
    }

    const auto server_config = app_config_->get_server_config();

    while (0 < count)
    {
        count = add_new_item(template_id, count, stacked, item->squares, *server_config);
    }

    return true;
}

int64_t celeritas::execute_change_item::add_to_existing_stacks(const int template_id, int64_t count, const int stacked)
{
    if (const auto* id_container = player_item_document_->get_id_container(template_id))
    {
        for (auto& element : std::ranges::reverse_view(*id_container))
        {
            if (auto optional_inventory_iter = player_item_document_->get_inventory_data(element);
                optional_inventory_iter)
            {
                auto& inventory = optional_inventory_iter.value()->second;
                inventory.add_count(count);
                if (stacked > 0 && inventory.get_count() > stacked)
                {
                    count = inventory.get_count() - stacked;
                    inventory.set_count(stacked);
                }
                else
                {
                    return 0;
                }

                inventory_data_.emplace(inventory.get_item_id(), inventory);
            }
        }
    }
    return count;
}

int64_t celeritas::execute_change_item::remove_from_existing_stacks(const int template_id, int64_t count)
{
    if (auto* id_container = player_item_document_->get_id_container(template_id))
    {
        for (auto id_iter = id_container->rbegin(); id_iter != id_container->rend();)
        {
            if (auto optional_inventory_iter = player_item_document_->get_inventory_data(*id_iter);
                optional_inventory_iter)
            {
                auto& inventory = optional_inventory_iter.value()->second;
                if (inventory.get_count() >= -count)
                {
                    inventory.add_count(count);
                    inventory_data_.emplace(inventory.get_item_id(), inventory);
                    return 0;
                }

                count += inventory.get_count();
                player_item_document_->remove_inventory_data(inventory.get_item_id());
                id_iter = std::make_reverse_iterator(id_container->erase(std::next(id_iter).base()));
                id_.emplace_back(inventory.get_item_id());
                continue;
            }
            ++id_iter;
        }
    }
    return count;
}

int64_t celeritas::execute_change_item::add_new_item(const int template_id, int64_t count, const int stacked, const bool squares, const server_config& server_config)
{
    const auto item_id = snowflake_generator::get_instance().generate(server_config.get_datacenter_id(), server_config.get_worker_id());
    const auto current_count = 0 < stacked && stacked < count ? stacked : count;
    count -= current_count;

    const inventory_data inventory_data{ item_id, template_id, current_count, player_item_document_->get_next_position(squares) };
    player_item_document_->add_inventory_data(inventory_data);

    inventory_data_.emplace(inventory_data.get_item_id(), inventory_data);
    return count;
}