#include "player_time_refresh.h"
#include "common/common_constant.h"
#include "common/core/celeritas_error.h"
#include "common/core/enum_cast.h"
#include "common/core/time_helper.h"
#include "database/basic/basis_database.tpp"
#include "player/time/time_refresh_type.h"

celeritas::player_time_refresh::player_time_refresh() noexcept
    : time_refresh_type_{}, parameter_{}, time_id_{}, component_{}, last_refresh_time_{}
{
}

celeritas::player_time_refresh::player_time_refresh(const time_refresh_type time_refresh_type, const int64_t parameter, const int64_t time_id, const player_component_type player_component_type)
    : time_refresh_type_{ time_refresh_type }, parameter_{ parameter }, time_id_{ time_id }, component_{ player_component_type }, last_refresh_time_{ time_helper::get_current_milliseconds() }
{
}

celeritas::time_refresh_type celeritas::player_time_refresh::get_time_refresh_type() const
{
    return time_refresh_type_;
}

void celeritas::player_time_refresh::set_time_refresh_type(const time_refresh_type timeRefreshType)
{
    time_refresh_type_ = timeRefreshType;
}

int64_t celeritas::player_time_refresh::get_parameter() const
{
    return parameter_;
}

void celeritas::player_time_refresh::set_parameter(const int64_t parameter)
{
    parameter_ = parameter;
}

int64_t celeritas::player_time_refresh::get_time_id() const
{
    return time_id_;
}

void celeritas::player_time_refresh::set_time_id(const int64_t time_id)
{
    time_id_ = time_id;
}

celeritas::player_time_refresh::component_container celeritas::player_time_refresh::get_component() const
{
    return component_;
}

void celeritas::player_time_refresh::set_component(const component_container& component)
{
    component_ = component;
}

void celeritas::player_time_refresh::add_component(player_component_type component)
{
    component_.emplace_back(component);
}

void celeritas::player_time_refresh::remove_component(const player_component_type playerComponent)
{
    std::erase(component_, playerComponent);
}

int64_t celeritas::player_time_refresh::get_last_refresh_time() const
{
    return last_refresh_time_;
}

void celeritas::player_time_refresh::set_last_refresh_time(const int64_t last_refresh_time)
{
    last_refresh_time_ = last_refresh_time;
}

bool celeritas::player_time_refresh::is_can_refresh() const
{
    return get_next_refresh_time() <= time_helper::get_current_milliseconds();
}

int64_t celeritas::player_time_refresh::get_next_refresh_time() const
{
    switch (time_refresh_type_)
    {
        case time_refresh_type::daily:
        {
            return time_helper::get_next_day_start_milliseconds_with_offset(last_refresh_time_, parameter_);
        }
        case time_refresh_type::weekly:
        {
            return time_helper::get_next_week_start_milliseconds_with_offset(last_refresh_time_, parameter_);
        }
        case time_refresh_type::monthly:
        {
            return time_helper::get_next_month_start_milliseconds_with_offset(last_refresh_time_, parameter_);
        }
        case time_refresh_type::interval_duration:
        {
            return last_refresh_time_ + parameter_;
        }
        case time_refresh_type::absolute_point:
        {
            return parameter_;
        }
        default:
        {
            throw celeritas_error{"Invalid time refresh type."};
        }
    }
}

bool celeritas::player_time_refresh::is_default() const
{
    switch (time_refresh_type_)
    {
        case time_refresh_type::daily:
        {
            return parameter_ % (hour_seconds * milliseconds) == 0;
        }
        case time_refresh_type::weekly:
        {
            return parameter_ == 0 || parameter_ == (day_seconds * 1 + hour_seconds * 0) * milliseconds;
        }
        case time_refresh_type::monthly:
        {
            return parameter_ == 0;
        }
        default:
        {
            return false;
        }
    }
}

celeritas::player_time_refresh::document_type celeritas::player_time_refresh::to_document_type() const
{
    document_type document{};

    document.emplace_back(time_refresh_type_description, enum_cast_underlying(time_refresh_type_));
    document.emplace_back(parameter_description, parameter_);
    document.emplace_back(time_id_description, time_id_);

    traits::int32_array_type basic{};
    for (const auto& element : component_)
    {
        basic.emplace_back(enum_cast_underlying(element));
    }
    document.emplace_back(component_description, basic);

    document.emplace_back(last_refresh_time_description, last_refresh_time_);

    return document;
}

celeritas::player_time_refresh celeritas::player_time_refresh::from_document(const document_type& document)
{
    player_time_refresh data{};

    for (const auto& element : document)
    {
        if (element.get_field_name() == time_refresh_type_description)
        {
            data.time_refresh_type_ = underlying_cast_enum<time_refresh_type>(element.get_value<database_data_type::int32_type>());
        }
        else if (element.get_field_name() == parameter_description)
        {
            data.parameter_ = element.get_value<database_data_type::int64_type>();
        }
        else if (element.get_field_name() == time_id_description)
        {
            data.time_id_ = element.get_value<database_data_type::int64_type>();
        }
        else if (element.get_field_name() == component_description)
        {
            const auto& result = element.get_value<database_data_type::int32_array_type>();
            component_container container{};
            for (const auto& component_type : result)
            {
                container.emplace_back(underlying_cast_enum<player_component_type>(component_type));
            }
            data.component_ = container;
        }
        else if (element.get_field_name() == last_refresh_time_description)
        {
            data.last_refresh_time_ = element.get_value<database_data_type::int64_type>();
        }
    }

    return data;
}
