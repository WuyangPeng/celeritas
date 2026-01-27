#include "item_selected_data.h"
#include "common/core/enum_cast.h"
#include "database/basic/basis_database.tpp"

celeritas::item_selected_data::item_selected_data(const int64_t id, const config::item_type item_type, const int child_type, const int64_t selected_id)
    : id_{ id }, item_type_{ item_type }, child_type_{ child_type }, selected_id_{ selected_id }
{
}

celeritas::item_selected_data::document_type celeritas::item_selected_data::to_document_type() const
{
    document_type document{};

    document.emplace_back(id_description, id_);
    document.emplace_back(item_type_description, enum_cast_underlying(item_type_));
    document.emplace_back(child_type_description, child_type_);
    document.emplace_back(selected_id_description, selected_id_);

    return document;
}

celeritas::item_selected_data celeritas::item_selected_data::from_document(const document_type& document)
{
    item_selected_data item_selected_data{};

    for (const auto& element : document)
    {
        if (element.get_field_name() == id_description)
        {
            item_selected_data.id_ = element.get_value<database_data_type::int64_type>();
        }
        else if (element.get_field_name() == item_type_description)
        {
            item_selected_data.item_type_ = underlying_cast_enum<config::item_type>(element.get_value<database_data_type::int32_type>());
        }
        else if (element.get_field_name() == child_type_description)
        {
            item_selected_data.child_type_ = element.get_value<database_data_type::int32_type>();
        }
        else if (element.get_field_name() == selected_id_description)
        {
            item_selected_data.selected_id_ = element.get_value<database_data_type::int64_type>();
        }
    }

    return item_selected_data;
}

int64_t celeritas::item_selected_data::get_id() const
{
    return id_;
}

void celeritas::item_selected_data::set_id(const int64_t id)
{
    id_ = id;
}

celeritas::config::item_type celeritas::item_selected_data::get_item_type() const
{
    return item_type_;
}

void celeritas::item_selected_data::set_item_type(const config::item_type itemType)
{
    item_type_ = itemType;
}

int celeritas::item_selected_data::get_child_type() const
{
    return child_type_;
}

void celeritas::item_selected_data::set_child_type(const int childType)
{
    child_type_ = childType;
}

int64_t celeritas::item_selected_data::get_selected_id() const
{
    return selected_id_;
}

void celeritas::item_selected_data::set_selected_id(const int64_t selectedId)
{
    selected_id_ = selectedId;
}