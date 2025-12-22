#include "player_item_document.h"
#include "common/celeritas_error.h"
#include "common/logger.h"
#include "common/snowflake_generator.h"
#include "config/app_config.h"
#include "config/game_config/game_config.h"
#include "config/game_config/game_tables.h"
#include "config/game_config/item_config.h"

#include <ranges>

void celeritas::player_item_document::set_item(traits::param_type::document_array_type item_document)
{
    for (const auto& element : item_document)
    {
        auto inventory_data = inventory_data::from_json_string(element);
        add_inventory_data(inventory_data);
    }
}

celeritas::traits::document_array_type celeritas::player_item_document::get_item() const
{
    traits::document_array_type documents{};
    for (auto& element : inventory_data_ | std::views::values)
    {
        documents.emplace_back(element.to_json_string());
    }

    return documents;
}

bool celeritas::player_item_document::change_item(const const_app_config_shared_ptr& app_config, const int template_id, int64_t count)
{
    if (count == 0)
    {
        return false;
    }

    const auto game_tables = game_config::get_instance().get_game_tables();
    const auto item = game_tables->get_item_config()->get(template_id);
    if (!item)
    {
        throw celeritas_error{ "item not found,template id = {}", template_id };
    }

    const auto stacked = (*item)->get_stacked();

    if (const auto template_iter = template_data_.find(template_id);
        template_iter != template_data_.cend())
    {
        for (auto id_iter = template_iter->second.rbegin(); id_iter != template_iter->second.rend();)
        {
            if (auto inventory_iter = inventory_data_.find(*id_iter);
                inventory_iter != inventory_data_.cend())
            {
                if (0 < count)
                {
                    inventory_iter->second.add_count(count);
                    if (stacked > 0 && inventory_iter->second.get_count() > stacked)
                    {
                        count = inventory_iter->second.get_count() - stacked;
                        inventory_iter->second.set_count(stacked);
                    }
                    else
                    {
                        count = 0;
                        break;
                    }
                }
                else
                {
                    if (inventory_iter->second.get_count() >= -count)
                    {
                        inventory_iter->second.add_count(count);
                        count = 0;
                        break;
                    }

                    inventory_data_.erase(inventory_iter->second.get_item_id());
                    id_iter = std::make_reverse_iterator(template_iter->second.erase(std::next(id_iter).base()));
                    continue;
                }

                ++id_iter;
            }
        }
    }

    if (count < 0)
    {
        LOG_CHANNEL(player_channel, error) << "item count is error ,template id = " << template_id << ",count = " << count;
        return true;
    }

    const auto server_config = app_config->get_server_config();

    while (0 < count)
    {
        const auto item_id = snowflake_generator::get_instance().generate(server_config.get_datacenter_id(), server_config.get_worker_id());
        const auto current_count = 0 < stacked && stacked < count ? stacked : count;
        count -= current_count;

        inventory_data inventory_data{ item_id, template_id, current_count, get_next_position((*item)->is_squares()) };
        add_inventory_data(inventory_data);
    }

    return true;
}

bool celeritas::player_item_document::can_consume_item(const int template_id, const int64_t count) const
{
    return get_count(template_id) >= count;
}

int64_t celeritas::player_item_document::get_count(const int template_id) const
{
    auto result = 0LL;
    if (const auto template_iter = template_data_.find(template_id);
        template_iter != template_data_.cend())
    {
        for (const auto id_iter = template_iter->second.rbegin(); id_iter != template_iter->second.rend();)
        {
            if (auto inventory_iter = inventory_data_.find(*id_iter);
                inventory_iter != inventory_data_.cend())
            {
                result += inventory_iter->second.get_count();
            }
        }
    }

    return result;
}

bool celeritas::player_item_document::can_consume_item(const item_container& item) const
{
    for (const auto& element : item)
    {
        if (!can_consume_item(element.get_template_id(), element.get_count()))
        {
            return false;
        }
    }
    return true;
}

bool celeritas::player_item_document::change_item(const const_app_config_shared_ptr& app_config, const item_container& item)
{
    auto result = false;
    for (const auto& element : item)
    {
        if (change_item(app_config, element.get_template_id(), element.get_count()))
        {
            result = true;
        }
    }
    return result;
}

int celeritas::player_item_document::get_next_position(const bool is_squares) const
{
    if (is_squares)
    {
        auto position = 0;
        for (const auto element : position_data_)
        {
            if (element == 0)
            {
                return position;
            }

            ++position;
        }

        return position_data_.size();
    }

    return -1;
}

void celeritas::player_item_document::add_inventory_data(const inventory_data& inventory_data)
{
    inventory_data_.emplace(inventory_data.get_item_id(), inventory_data);

    template_data_[inventory_data.get_template_id()].emplace_back(inventory_data.get_item_id());

    if (inventory_data.get_position() < 0)
    {
        return;
    }

    if (position_data_.size() <= inventory_data.get_position())
    {
        position_data_.resize(inventory_data.get_position() + 1);
    }

    position_data_.at(inventory_data.get_position()) = inventory_data.get_item_id();
    return;
}