#include "red_dots.h"
#include "common/core/enum_cast.h"
#include "common/core/time_helper.h"
#include "database/basic/basis_database.tpp"

celeritas::red_dots::red_dots()
    : node_id_{}, state_{}, last_value_{}, update_time_{}
{
}

celeritas::red_dots::red_dots(const red_dot_type node_id, const bool state)
    : node_id_{ node_id }, state_{ state }, last_value_{}, update_time_{ time_helper::get_current_milliseconds() }
{
}

celeritas::config::red_dot_type celeritas::red_dots::get_node_id() const
{
    return node_id_;
}

void celeritas::red_dots::set_node_id(const red_dot_type nodeId)
{
    node_id_ = nodeId;
}

bool celeritas::red_dots::is_state() const
{
    return state_;
}

void celeritas::red_dots::set_state(const bool state)
{
    state_ = state;
}

int64_t celeritas::red_dots::get_last_value() const
{
    return last_value_;
}

void celeritas::red_dots::set_last_value(const int64_t lastValue)
{
    last_value_ = lastValue;
}

int64_t celeritas::red_dots::get_update_time() const
{
    return update_time_;
}

void celeritas::red_dots::set_update_time(const int64_t updateTime)
{
    update_time_ = updateTime;
}

celeritas::red_dots::document_type celeritas::red_dots::to_document_type() const
{
    document_type document{};

    document.emplace_back(node_id_description, enum_cast_underlying(node_id_));
    document.emplace_back(state_description, state_);
    document.emplace_back(last_value_description, last_value_);
    document.emplace_back(update_time_description, update_time_);

    return document;
}

celeritas::red_dots celeritas::red_dots::from_document(const document_type& document)
{
    red_dots red_dots{};

    for (const auto& element : document)
    {
        if (element.get_field_name() == node_id_description)
        {
            red_dots.set_node_id(underlying_cast_enum<red_dot_type>(element.get_value<database_data_type::int32_type>()));
        }
        else if (element.get_field_name() == state_description)
        {
            red_dots.set_state(element.get_value<database_data_type::bool_type>());
        }
        else if (element.get_field_name() == last_value_description)
        {
            red_dots.set_last_value(element.get_value<database_data_type::int64_type>());
        }
        else if (element.get_field_name() == update_time_description)
        {
            red_dots.set_update_time(element.get_value<database_data_type::int64_type>());
        }
    }

    return red_dots;
}
