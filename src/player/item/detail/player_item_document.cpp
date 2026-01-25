#include "player_item_document.h"
#include "boost/numeric/conversion/cast.hpp"
#include "common/core/celeritas_error.h"
#include "common/logging/logger.h"
#include "common/core/snowflake_generator.h"
#include "config/aggregate/app_config.h"
#include "config/game/game_config.h"
#include "config/game/game_tables.h"
#include "database/basic/basis_database.tpp"

#include <ranges>

void celeritas::player_item_document::set_item(traits::param_type::document_array_type item_document)
{
    for (const auto& element : item_document)
    {
        auto inventory_data = inventory_data::from_document(element);
        add_inventory_data(inventory_data);
    }
}

celeritas::traits::document_array_type celeritas::player_item_document::get_item() const
{
    traits::document_array_type documents{};
    for (auto& element : inventory_data_ | std::views::values)
    {
        documents.emplace_back(element.to_document_type());
    }

    return documents;
}

bool celeritas::player_item_document::change_item(const const_app_config_shared_ptr& app_config, const int template_id, int64_t count)
{
    if (count == 0)
    {
        return false;
    }

    const auto item = get_item_config(template_id);
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

    const auto server_config = app_config->get_server_config();

    while (0 < count)
    {
        count = add_new_item(template_id, count, stacked, item->squares, *server_config);
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
    if (const auto* id_container = get_id_container(template_id))
    {
        for (const auto& element : *id_container)
        {
            if (auto inventory_iter = inventory_data_.find(element);
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
    return std::ranges::all_of(item, [this](const auto& element) {
        return can_consume_item(element.get_template_id(), element.get_count());
    });
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

        return boost::numeric_cast<int>(position_data_.size());
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
}

int64_t celeritas::player_item_document::add_to_existing_stacks(const int template_id, int64_t count, const int stacked)
{
    if (const auto* id_container = get_id_container(template_id))
    {
        for (auto& element : std::ranges::reverse_view(*id_container))
        {
            if (auto inventory_iter = inventory_data_.find(element);
                inventory_iter != inventory_data_.cend())
            {
                inventory_iter->second.add_count(count);
                if (stacked > 0 && inventory_iter->second.get_count() > stacked)
                {
                    count = inventory_iter->second.get_count() - stacked;
                    inventory_iter->second.set_count(stacked);
                }
                else
                {
                    return 0;
                }
            }
        }
    }
    return count;
}

int64_t celeritas::player_item_document::remove_from_existing_stacks(const int template_id, int64_t count)
{
    if (auto* id_container = get_id_container(template_id))
    {
        for (auto id_iter = id_container->rbegin(); id_iter != id_container->rend();)
        {
            if (auto inventory_iter = inventory_data_.find(*id_iter);
                inventory_iter != inventory_data_.cend())
            {
                if (inventory_iter->second.get_count() >= -count)
                {
                    inventory_iter->second.add_count(count);
                    return 0;
                }

                count += inventory_iter->second.get_count();
                inventory_data_.erase(inventory_iter->second.get_item_id());
                id_iter = std::make_reverse_iterator(id_container->erase(std::next(id_iter).base()));
                continue;
            }
            ++id_iter;
        }
    }
    return count;
}

int64_t celeritas::player_item_document::add_new_item(const int template_id, int64_t count, const int stacked, const bool squares, const server_config& server_config)
{
    const auto item_id = snowflake_generator::get_instance().generate(server_config.get_datacenter_id(), server_config.get_worker_id());
    const auto current_count = 0 < stacked && stacked < count ? stacked : count;
    count -= current_count;

    const inventory_data inventory_data{ item_id, template_id, current_count, get_next_position(squares) };
    add_inventory_data(inventory_data);

    return count;
}

celeritas::player_item_document::const_item_config_shared_ptr celeritas::player_item_document::get_item_config(int template_id)
{
    const auto game_tables = game_config::get_instance().get_game_tables();
    const auto item = game_tables->get_tables()->item_config_container.get(template_id);
    if (!item)
    {
        throw celeritas_error{ "item not found,template id = {}", template_id };
    }

    return *item;
}

celeritas::player_item_document::id_container* celeritas::player_item_document::get_id_container(const int template_id)
{
    return const_cast<id_container*>(static_cast<const class_type*>(this)->get_id_container(template_id));
}

const celeritas::player_item_document::id_container* celeritas::player_item_document::get_id_container(const int template_id) const
{
    if (const auto template_iter = template_data_.find(template_id);
        template_iter != template_data_.cend())
    {
        return &template_iter->second;
    }

    return nullptr;
}