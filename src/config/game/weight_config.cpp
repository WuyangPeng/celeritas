#include "weight_config.h"
#include "common/core/celeritas_error.h"

#include <ranges>

celeritas::weight_config::weight_config(const surname_config_container_type& surname_config_container, const name_config_container_type& name_config_container)
    : surname_weight_{}, name_weight_{}
{
    init_name_config(name_config_container);
    init_surname_config(surname_config_container);
}

std::string celeritas::weight_config::get_surname(const surname_config_container_type& surname_config_container) const
{
    const auto id = surname_weight_.get_random_id_by_weight();

    const auto surname = surname_config_container.get(id);

    return (*surname)->name;
}

std::string celeritas::weight_config::get_name(sex_type sex_type, const name_config_container_type& name_config_container) const
{
    const auto iter = name_weight_.find(sex_type);
    if (iter == name_weight_.cend())
    {
        throw celeritas_error{ "name weight is not exist ,sex type = {}", static_cast<int>(sex_type) };
    }

    const auto id = iter->second.get_random_id_by_weight();

    if (const auto name = name_config_container.get(id))
    {
        return (*name)->name;
    }

    throw celeritas_error{ "name is not exist ,id = {}", id };
}

void celeritas::weight_config::init_surname_config(const surname_config_container_type& surname_config_container)
{
    surname_weight_.clear();

    for (const auto& [id, surname] : surname_config_container.getDataMap())
    {
        surname_weight_.add_element(id, surname->weight);
    }
}

void celeritas::weight_config::init_name_config(const name_config_container_type& name_config_container)
{
    name_weight_.clear();

    for (const auto& name : name_config_container.getDataMap() | std::views::values)
    {
        init_name_config(*name);
    }
}

void celeritas::weight_config::init_name_config(const name_config_type& config)
{
    name_weight_[sex_type::none].add_element(config.id, config.weight);
    if (config.sexType != sex_type::female)
    {
        name_weight_[sex_type::male].add_element(config.id, config.weight);
    }
    if (config.sexType != sex_type::male)
    {
        name_weight_[sex_type::female].add_element(config.id, config.weight);
    }
}