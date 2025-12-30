#include "develop_data.h"
#include "common/core_utilities/time_helper.h"
#include "database/basis_database.tpp"

#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/basic/document.hpp>

celeritas::develop_data::develop_data(const int system_id, const int64_t instance_id)
    : system_id_{ system_id }, instance_id_{ instance_id }, level_{ 1 }, exp_{ 0 }, updated_time_{ time_helper::get_current_milliseconds() }
{
}

int celeritas::develop_data::get_system_id() const
{
    return system_id_;
}

void celeritas::develop_data::set_system_id(const int systemId)
{
    system_id_ = systemId;
}

int64_t celeritas::develop_data::get_instance_id() const
{
    return instance_id_;
}

void celeritas::develop_data::set_instance_id(const int64_t instanceId)
{
    instance_id_ = instanceId;
}

int celeritas::develop_data::get_level() const
{
    return level_;
}

void celeritas::develop_data::set_level(const int level)
{
    level_ = level;
}

void celeritas::develop_data::add_level()
{
    ++level_;
    updated_time_ = time_helper::get_current_milliseconds();
}

int64_t celeritas::develop_data::get_exp() const
{
    return exp_;
}

void celeritas::develop_data::set_exp(const int64_t exp)
{
    exp_ = exp;
}

int64_t celeritas::develop_data::get_updated_time() const
{
    return updated_time_;
}

void celeritas::develop_data::set_updated_time(const int64_t updated_time)
{
    updated_time_ = updated_time;
}

void celeritas::develop_data::clear()
{
    level_ = 0;
    exp_ = 0;
    updated_time_ = time_helper::get_current_milliseconds();
}

celeritas::develop_data::document_type celeritas::develop_data::to_document_type() const
{
    document_type document{};

    document.emplace_back(system_id_description, system_id_);
    document.emplace_back(instance_id_description, instance_id_);
    document.emplace_back(level_description, level_);
    document.emplace_back(exp_description, exp_);
    document.emplace_back(updated_time_description, updated_time_);

    return document;
}

celeritas::develop_data celeritas::develop_data::from_document(const document_type& document)
{
    develop_data develop_data{};

    for (const auto& element : document)
    {
        if (element.get_field_name() == system_id_description)
        {
            develop_data.set_system_id(element.get_value<database_data_type::int32_type>());
        }
        if (element.get_field_name() == instance_id_description)
        {
            develop_data.set_instance_id(element.get_value<database_data_type::int64_type>());
        }
        if (element.get_field_name() == level_description)
        {
            develop_data.set_level(element.get_value<database_data_type::int32_type>());
        }
        if (element.get_field_name() == exp_description)
        {
            develop_data.set_exp(element.get_value<database_data_type::int64_type>());
        }
        if (element.get_field_name() == updated_time_description)
        {
            develop_data.set_updated_time(element.get_value<database_data_type::int64_type>());
        }
    }

    return develop_data;
}
