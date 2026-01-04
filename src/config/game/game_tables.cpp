#include "container_config.tpp"
#include "game_tables.h"
#include "common/core/random_helper.h"

#include <ranges>

celeritas::game_tables::game_tables(const_tables_shared_ptr tables)
    : tables_{ std::move(tables) },
      surname_weight_{},
      name_weight_{}
{
    init_surname_config();
    init_name_config();
}

std::string celeritas::game_tables::get_surname() const
{
    const auto index = random_helper::get_random_index_by_weight(surname_weight_.get_weights());

    const auto id = surname_weight_.get_id(index);

    const auto surname = tables_->surname_config_container.get(id);

    return (*surname)->name;
}

std::string celeritas::game_tables::get_name(const sex_type sex_type) const
{
    const auto iter = name_weight_.find(sex_type);
    if (iter == name_weight_.cend())
    {
        throw celeritas_error{ "name weight is not exist ,sex type = {}", static_cast<int>(sex_type) };
    }

    const auto index = random_helper::get_random_index_by_weight(iter->second.get_weights());

    const auto id = iter->second.get_id(index);

    if (const auto name = tables_->name_config_container.get(id))
    {
        return (*name)->name;
    }

    throw celeritas_error{ "name is not exist ,id = {}", id };
}

celeritas::game_tables::const_tables_shared_ptr celeritas::game_tables::get_tables() const
{
    return tables_;
}

void celeritas::game_tables::init_surname_config()
{
    surname_weight_.clear();

    for (const auto& [id, surname] : tables_->surname_config_container.getDataMap())
    {
        surname_weight_.add_element(id, surname->weight);
    }
}

void celeritas::game_tables::init_name_config()
{
    name_weight_.clear();

    for (const auto& name : tables_->name_config_container.getDataMap() | std::views::values)
    {
        init_name_config(*name);
    }
}

void celeritas::game_tables::init_name_config(const name_config_type& config)
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

