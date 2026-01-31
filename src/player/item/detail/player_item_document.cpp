#include "execute_change_item.h"
#include "player_item_document.h"
#include "boost/numeric/conversion/cast.hpp"
#include "common/core/celeritas_error.h"
#include "common/logging/logger.h"
#include "config/game/game_config.h"
#include "config/game/game_tables.h"
#include "database/basic/basis_database.tpp"
#include "initializer/initializer_constant.h"
#include "message/basic/header.h"
#include "proto/celeritas.pb.h"

#include <ranges>

celeritas::player_item_document::player_item_document(player_state* player_state)
    : inventory_data_{},
      template_data_{},
      position_data_{},
      player_state_{ player_state }
{
}

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

bool celeritas::player_item_document::change_item(const const_app_config_shared_ptr& app_config, const int template_id, const int64_t count)
{
    execute_change_item execute{ this, app_config, template_id, count };

    execute.execute();
    execute.send_message();

    return execute.is_change();
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
    execute_change_item execute{ this, app_config, item };

    execute.execute();
    execute.send_message();

    return execute.is_change();
}

celeritas::player_item_document::optional_inventory_data_container_iter celeritas::player_item_document::get_inventory_data(const int64_t item_id)
{
    if (const auto iter = inventory_data_.find(item_id);
        iter != inventory_data_.cend())
    {
        return iter;
    }

    return std::nullopt;
}

void celeritas::player_item_document::on_dependencies_ready()
{
    send_item_message(true, inventory_data_);
}

void celeritas::player_item_document::remove_inventory_data(const int64_t item_id)
{
    inventory_data_.erase(item_id);
}

bool celeritas::player_item_document::has_item(const int64_t item_id) const
{
    return inventory_data_.contains(item_id);
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

void celeritas::player_item_document::send_item_message(const bool is_login, const inventory_data_container& inventory) const
{
    const header header{ player_state_->get_user_id() };

    proto::celeritas response{};
    auto* item_response = response.mutable_celeritas_response()->mutable_client()->mutable_player()->mutable_item()->mutable_item();
    item_response->set_is_login(is_login);
    for (const auto& element : inventory | std::views::values)
    {
        auto* inventory_data = item_response->add_inventory();

        inventory_data->set_item_id(element.get_item_id());
        inventory_data->set_template_id(element.get_template_id());
        inventory_data->set_count(element.get_count());
        inventory_data->set_position(element.get_position());

        switch (const auto custom_data = element.get_custom_data();
            custom_data.get_kind())
        {
            case config::item_type::custom:
            {
                inventory_data->mutable_custom();
            }
            break;
            case config::item_type::consumable:
            {
                auto* consumable = inventory_data->mutable_consumable();
                const auto* data = custom_data.get_consumable();

                consumable->set_expire_time(data->get_expire_time());
            }
            break;
            case config::item_type::equipment:
            {
                auto* equipment = inventory_data->mutable_equipment();
                const auto* data = custom_data.get_equipment();

                equipment->set_durability(data->get_durability());
                equipment->set_strength(data->get_strength());
            }
            break;
            case config::item_type::avatar:
            {
                inventory_data->mutable_avatar();
            }
            break;
            case config::item_type::frame:
            {
                inventory_data->mutable_frame();
            }
            break;
            case config::item_type::title:
            {
                inventory_data->mutable_title();
            }
            break;
            case config::item_type::hero:
            {
                inventory_data->mutable_hero();
            }
            break;
            default:
            {
                inventory_data->mutable_custom();
            }
            break;
        }
    }

    if (!player_state_->write(gateway_type.data(), player_state_->get_instance_id(), header, response))
    {
        LOG_CHANNEL(player_channel, error) << "send message error.";
    }
}

void celeritas::player_item_document::send_delete_item_message(const inventory_data_container& inventory) const
{
    const header header{ player_state_->get_user_id() };

    proto::celeritas response{};
    auto* item_response = response.mutable_celeritas_response()->mutable_client()->mutable_player()->mutable_item()->mutable_item_delete();
    for (const auto& element : inventory | std::views::values)
    {
        auto* data = item_response->add_data();
        data->set_item_id(element.get_item_id());
        data->set_template_id(element.get_template_id());
    }

    if (!player_state_->write(gateway_type.data(), player_state_->get_instance_id(), header, response))
    {
        LOG_CHANNEL(player_channel, error) << "send message error.";
    }
}

