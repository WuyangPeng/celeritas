#include "container_config.tpp"
#include "game_tables.h"
#include "common/random_helper.h"

celeritas::game_tables::game_tables(const_tables_shared_ptr tables)
    : tables_{ std::move(tables) },
      surname_weight_{},
      name_weight_{},
      red_dot_{ std::make_shared<red_dot_container_config>() },
      develop_{ std::make_shared<develop_container_config>() }
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

    const auto name = tables_->name_config_container.get(id);

    return (*name)->name;
}

celeritas::game_tables::const_red_dot_container_shared_ptr celeritas::game_tables::get_red_dot_config() const
{
    return red_dot_;
}

void celeritas::game_tables::set_red_dot_config(const const_red_dot_container_shared_ptr& red_dot)
{
    red_dot_ = red_dot;
}

celeritas::game_tables::const_develop_container_shared_ptr celeritas::game_tables::get_develop_config() const
{
    return develop_;
}

void celeritas::game_tables::set_develop_config(const const_develop_container_shared_ptr& develop)
{
    develop_ = develop;
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

    auto& null_name_weight_ = name_weight_[sex_type::none];
    auto& male_name_weight = name_weight_[sex_type::male];
    auto& female_name_weight = name_weight_[sex_type::female];

    for (const auto& [id, name] : tables_->name_config_container.getDataMap())
    {
        null_name_weight_.add_element(id, name->weight);
        if (name->sexType != sex_type::female)
        {
            male_name_weight.add_element(id, name->weight);
        }
        if (name->sexType != sex_type::male)
        {
            female_name_weight.add_element(id, name->weight);
        }
    }
}

