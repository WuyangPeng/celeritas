#include "inventory_data.h"

#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/basic/document.hpp>

celeritas::inventory_data::inventory_data(const int64_t item_id, const int template_id, const int64_t count, const int position)
    : item_id_{ item_id }, template_id_{ template_id }, count_{ count }, position_{ position }
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

void celeritas::inventory_data::set_custom_data(const document_view_type& document)
{
    custom_data_.set_document(document);
}

void celeritas::inventory_data::add_count(const int64_t count)
{
    count_ += count;
}

void celeritas::inventory_data::reduce_count(const int64_t count)
{
    count_ -= count;
}

std::string celeritas::inventory_data::to_json_string() const
{
    bsoncxx::builder::basic::document builder{};
    builder.append(bsoncxx::builder::basic::kvp(std::string{ item_id_description }, item_id_));
    builder.append(bsoncxx::builder::basic::kvp(std::string{ template_id_description }, template_id_));
    builder.append(bsoncxx::builder::basic::kvp(std::string{ count_description }, count_));
    builder.append(bsoncxx::builder::basic::kvp(std::string{ position_description }, position_));
    builder.append(bsoncxx::builder::basic::kvp(std::string{ custom_data_description }, custom_data_.to_document_type()));

    return bsoncxx::to_json(builder.view());
}

celeritas::inventory_data celeritas::inventory_data::from_json_string(const std::string& json_string)
{
    const auto parsed_view = bsoncxx::from_json(json_string);

    inventory_data inventory_data{};

    inventory_data.set_item_id(parsed_view[item_id_description].type() == bsoncxx::type::k_int32 ? parsed_view[item_id_description].get_int32().value : parsed_view[item_id_description].get_int64().value);
    inventory_data.set_template_id(parsed_view[template_id_description].get_int32().value);
    inventory_data.set_count(parsed_view[count_description].type() == bsoncxx::type::k_int32 ? parsed_view[count_description].get_int32().value : parsed_view[count_description].get_int64().value);
    inventory_data.set_position(parsed_view[position_description].get_int32().value);
    inventory_data.set_custom_data(parsed_view[custom_data_description].get_document().value);

    return inventory_data;
}