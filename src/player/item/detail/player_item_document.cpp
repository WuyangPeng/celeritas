#include "execute_change_item.h"
#include "player_item_document.h"
#include "boost/numeric/conversion/cast.hpp"
#include "common/core/celeritas_error.h"
#include "common/logging/logger.h"
#include "config/game/game_config.h"
#include "config/game/game_tables.h"
#include "database/basic/basis_database.tpp"
#include "database/document/resource_data.h"
#include "database/document/soldier_data.h"
#include "database/document/machine_data.h"
#include "database/document/skill_book_data.h"
#include "database/document/blueprint_data.h"
#include "database/document/gift_box_data.h"
#include "database/document/treasure_data.h"
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

celeritas::player_item_document::bool_awaitable_type celeritas::player_item_document::change_item(const const_app_config_shared_ptr& app_config,
                                                                                                  const int template_id,
                                                                                                  const int64_t count,
                                                                                                  const bool is_login)
{
    execute_change_item execute{ player_state_, this, app_config, template_id, count };

    execute.execute();
    execute.send_message();
    co_await execute.execute_develop();
    execute.send_develop_message();

    co_await execute.trigger_item_event(is_login);

    co_return execute.is_change();
}

bool celeritas::player_item_document::can_consume_item(const int template_id, const int64_t count) const
{
    auto result = 0LL;
    if (const auto* id_container = get_id_container(template_id))
    {
        for (const auto& element : *id_container)
        {
            if (auto inventory_iter = inventory_data_.find(element);
                inventory_iter != inventory_data_.cend())
            {
                if (!inventory_iter->second.is_locked())
                {
                    result += inventory_iter->second.get_count();
                }
            }
        }
    }

    return result >= count;
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

celeritas::player_item_document::bool_awaitable_type celeritas::player_item_document::change_item(const const_app_config_shared_ptr& app_config,
                                                                                                  const item_container& item,
                                                                                                  const bool is_login)
{
    execute_change_item execute{ player_state_, this, app_config, item };

    execute.execute();

    if (!is_login)
    {
        execute.send_message();
    }

    co_await execute.execute_develop();
    if (!is_login)
    {
        execute.send_develop_message();
    }

    co_await execute.trigger_item_event(is_login);

    co_return execute.is_change();
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

void celeritas::player_item_document::send_initial_sync()
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

void celeritas::player_item_document::set_inventory_data_proto(proto_inventory_data* proto_data, const inventory_data& inventory_data)
{
    proto_data->set_item_id(inventory_data.get_item_id());
    proto_data->set_template_id(inventory_data.get_template_id());
    proto_data->set_count(inventory_data.get_count());
    proto_data->set_position(inventory_data.get_position());
    proto_data->set_is_locked(inventory_data.is_locked());

    switch (const auto custom_data = inventory_data.get_custom_data();
        custom_data.get_kind())
    {
        case config::item_type::custom:
        {
            proto_data->mutable_custom();
        }
        break;
        case config::item_type::consumable:
        {
            auto* consumable = proto_data->mutable_consumable();
            const auto* data = custom_data.get_consumable();

            consumable->set_expire_time(data->get_expire_time());
        }
        break;
        case config::item_type::equipment:
        {
            auto* equipment = proto_data->mutable_equipment();
            const auto* data = custom_data.get_equipment();

            equipment->set_durability(data->get_durability());
            equipment->set_strength(data->get_strength());
        }
        break;
        case config::item_type::avatar:
        {
            proto_data->mutable_avatar();
        }
        break;
        case config::item_type::frame:
        {
            proto_data->mutable_frame();
        }
        break;
        case config::item_type::title:
        {
            proto_data->mutable_title();
        }
        break;
        case config::item_type::hero:
        {
            proto_data->mutable_hero();
        }
        break;
        case config::item_type::exp:
        {
            proto_data->mutable_exp();
        }
        break;
        case config::item_type::building:
        {
            auto* building = proto_data->mutable_building();
            const auto* data = custom_data.get_building();

            building->set_level(data->get_level());
        }
        break;
        case config::item_type::resource:
        {
            proto_data->mutable_resource();
        }
        break;
        case config::item_type::soldier:
        {
            proto_data->mutable_soldier();
        }
        break;
        case config::item_type::machine:
        {
            proto_data->mutable_machine();
        }
        break;
        case config::item_type::skill_book:
        {
            proto_data->mutable_skill_book();
        }
        break;
        case config::item_type::blueprint:
        {
            proto_data->mutable_blueprint();
        }
        break;
        case config::item_type::gift_box:
        {
            proto_data->mutable_gift_box();
        }
        break;
        case config::item_type::treasure:
        {
            proto_data->mutable_treasure();
        }
        break;
        default:
        {
            proto_data->mutable_custom();
        }
        break;
    }
}

celeritas::inventory_data celeritas::player_item_document::get_inventory_data_by_proto(const proto_inventory_data* proto_data)
{
    inventory_data inventory_data{};

    inventory_data.set_item_id(proto_data->item_id());
    inventory_data.set_template_id(proto_data->template_id());
    inventory_data.set_count(proto_data->count());
    inventory_data.set_position(proto_data->position());
    inventory_data.set_is_locked(proto_data->is_locked());

    custom_data custom_data{};

    switch (proto_data->payload_case())
    {
        case proto_inventory_data::kCustom:
        {
        }
        break;
        case proto_inventory_data::kConsumable:
        {
            const auto& consumable_proto = proto_data->consumable();
            const consumable_data consumable{ consumable_proto.expire_time() };

            traits::document_type document{};
            document.emplace_back(custom_data::type_description, custom_data::consumable_description.data());
            document.emplace_back(custom_data::data_description, consumable.to_document_type());
            custom_data = custom_data::from_document(document);
        }
        break;
        case proto_inventory_data::kEquipment:
        {
            const auto& equipment_proto = proto_data->equipment();
            const equipment_data equipment{ equipment_proto.strength(), equipment_proto.durability() };

            traits::document_type document{};
            document.emplace_back(custom_data::type_description, custom_data::equipment_description.data());
            document.emplace_back(custom_data::data_description, equipment.to_document_type());
            custom_data = custom_data::from_document(document);
        }
        break;
        case proto_inventory_data::kAvatar:
        {
            const avatar_data avatar{};
            traits::document_type document{};
            document.emplace_back(custom_data::type_description, custom_data::avatar_description.data());
            document.emplace_back(custom_data::data_description, avatar.to_document_type());
            custom_data = custom_data::from_document(document);
        }
        break;
        case proto_inventory_data::kFrame:
        {
            const frame_data frame{};
            traits::document_type document{};
            document.emplace_back(custom_data::type_description, custom_data::frame_description.data());
            document.emplace_back(custom_data::data_description, frame.to_document_type());
            custom_data = custom_data::from_document(document);
        }
        break;
        case proto_inventory_data::kTitle:
        {
            const title_data title{};
            traits::document_type document{};
            document.emplace_back(custom_data::type_description, custom_data::title_description.data());
            document.emplace_back(custom_data::data_description, title.to_document_type());
            custom_data = custom_data::from_document(document);
        }
        break;
        case proto_inventory_data::kHero:
        {
            const hero_data hero{};
            traits::document_type document{};
            document.emplace_back(custom_data::type_description, custom_data::hero_description.data());
            document.emplace_back(custom_data::data_description, hero.to_document_type());
            custom_data = custom_data::from_document(document);
        }
        break;
        case proto_inventory_data::kExp:
        {
            const exp_data exp{};

            traits::document_type document{};
            document.emplace_back(custom_data::type_description, custom_data::exp_description.data());
            document.emplace_back(custom_data::data_description, exp.to_document_type());
            custom_data = custom_data::from_document(document);
        }
        break;
        case proto_inventory_data::kBuilding:
        {
            const auto& building_proto = proto_data->building();
            const building_data building{ building_proto.level() };

            traits::document_type document{};
            document.emplace_back(custom_data::type_description, custom_data::building_description.data());
            document.emplace_back(custom_data::data_description, building.to_document_type());
            custom_data = custom_data::from_document(document);
        }
        break;
        case proto_inventory_data::kResource:
        {
            const resource_data resource{};

            traits::document_type document{};
            document.emplace_back(custom_data::type_description, custom_data::resource_description.data());
            document.emplace_back(custom_data::data_description, resource.to_document_type());
            custom_data = custom_data::from_document(document);
        }
        break;
        case proto_inventory_data::kSoldier:
        {
            const soldier_data soldier{};

            traits::document_type document{};
            document.emplace_back(custom_data::type_description, custom_data::soldier_description.data());
            document.emplace_back(custom_data::data_description, soldier.to_document_type());
            custom_data = custom_data::from_document(document);
        }
        break;
        case proto_inventory_data::kMachine:
        {
            const machine_data machine{};

            traits::document_type document{};
            document.emplace_back(custom_data::type_description, custom_data::machine_description.data());
            document.emplace_back(custom_data::data_description, machine.to_document_type());
            custom_data = custom_data::from_document(document);
        }
        break;
        case proto_inventory_data::kSkillBook:
        {
            const skill_book_data skill_book{};

            traits::document_type document{};
            document.emplace_back(custom_data::type_description, custom_data::skill_book_description.data());
            document.emplace_back(custom_data::data_description, skill_book.to_document_type());
            custom_data = custom_data::from_document(document);
        }
        break;
        case proto_inventory_data::kBlueprint:
        {
            const blueprint_data blueprint{};

            traits::document_type document{};
            document.emplace_back(custom_data::type_description, custom_data::blueprint_description.data());
            document.emplace_back(custom_data::data_description, blueprint.to_document_type());
            custom_data = custom_data::from_document(document);
        }
        break;
        case proto_inventory_data::kGiftBox:
        {
            const gift_box_data gift_box{};

            traits::document_type document{};
            document.emplace_back(custom_data::type_description, custom_data::gift_box_description.data());
            document.emplace_back(custom_data::data_description, gift_box.to_document_type());
            custom_data = custom_data::from_document(document);
        }
        break;
        case proto_inventory_data::kTreasure:
        {
            const treasure_data treasure{};

            traits::document_type document{};
            document.emplace_back(custom_data::type_description, custom_data::treasure_description.data());
            document.emplace_back(custom_data::data_description, treasure.to_document_type());
            custom_data = custom_data::from_document(document);
        }
        break;
        case proto_inventory_data::PAYLOAD_NOT_SET:
        default:
        {
            break;
        }
    }

    inventory_data.set_custom_data(custom_data);

    return inventory_data;
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
        set_inventory_data_proto(inventory_data, element);
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

