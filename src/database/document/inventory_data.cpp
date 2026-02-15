#include "inventory_data.h"
#include "common/core/celeritas_error.h"
#include "config/game/game_config.h"
#include "config/game/game_tables.h"
#include "database/basic/basis_database.tpp"

celeritas::inventory_data::inventory_data(const int64_t item_id, const int template_id, const int64_t count, const int position)
    : item_id_{ item_id }, template_id_{ template_id }, count_{ count }, position_{ position }, custom_data_{ get_custom_data(template_id) }
{
}

int64_t celeritas::inventory_data::get_item_id() const
{
    return item_id_;
}

void celeritas::inventory_data::set_item_id(const int64_t item_id)
{
    item_id_ = item_id;
}

int celeritas::inventory_data::get_template_id() const
{
    return template_id_;
}

void celeritas::inventory_data::set_template_id(const int template_id)
{
    template_id_ = template_id;
}

int64_t celeritas::inventory_data::get_count() const
{
    return count_;
}

void celeritas::inventory_data::set_count(const int64_t count)
{
    count_ = count;
}

int celeritas::inventory_data::get_position() const
{
    return position_;
}

void celeritas::inventory_data::set_position(const int position)
{
    position_ = position;
}

celeritas::custom_data celeritas::inventory_data::get_custom_data() const
{
    return custom_data_;
}

void celeritas::inventory_data::set_custom_data(const custom_data& custom_data)
{
    custom_data_ = custom_data;
}

void celeritas::inventory_data::add_count(const int64_t count)
{
    count_ += count;
}

void celeritas::inventory_data::reduce_count(const int64_t count)
{
    count_ -= count;
}

celeritas::inventory_data::document_type celeritas::inventory_data::to_document_type() const
{
    document_type document{};

    document.emplace_back(item_id_description, item_id_);
    document.emplace_back(template_id_description, template_id_);
    document.emplace_back(count_description, count_);
    document.emplace_back(position_description, position_);
    document.emplace_back(custom_data_description, custom_data_.to_document_type());

    return document;
}

celeritas::inventory_data celeritas::inventory_data::from_document(const document_type& document)
{
    inventory_data inventory_data{};

    for (const auto& element : document)
    {
        if (element.get_field_name() == item_id_description)
        {
            inventory_data.set_item_id(element.get_value<database_data_type::int64_type>());
        }
        else if (element.get_field_name() == template_id_description)
        {
            inventory_data.set_template_id(element.get_value<database_data_type::int32_type>());
        }
        else if (element.get_field_name() == count_description)
        {
            inventory_data.set_count(element.get_value<database_data_type::int64_type>());
        }
        else if (element.get_field_name() == position_description)
        {
            inventory_data.set_position(element.get_value<database_data_type::int32_type>());
        }
        else if (element.get_field_name() == custom_data_description)
        {
            const auto& database = element.get_value<database_data_type::document_type>();
            const auto custom_data = custom_data::from_document(database);
            inventory_data.set_custom_data(custom_data);
        }
    }

    return inventory_data;
}

std::string_view celeritas::inventory_data::get_custom_data(const int template_id)
{
    const auto item_config = game_config::get_instance().get_game_tables()->get_tables()->item_config_container.get(template_id);

    switch ((*item_config)->itemType)
    {
        case config::item_type::none:
        case config::item_type::custom:
            return custom_data::custom_description;
        case config::item_type::consumable:
            return custom_data::consumable_description;
        case config::item_type::equipment:
            return custom_data::equipment_description;
        case config::item_type::avatar:
            return custom_data::avatar_description;
        case config::item_type::frame:
            return custom_data::frame_description;
        case config::item_type::title:
            return custom_data::title_description;
        case config::item_type::hero:
            return custom_data::hero_description;
        default:
            throw celeritas_error("invalid item type");
    }
}
