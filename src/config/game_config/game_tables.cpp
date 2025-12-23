#include "container_config.tpp"
#include "game_tables.h"
#include "name_config.h"
#include "sex_type.h"
#include "surname_config.h"
#include "common/random_helper.h"

celeritas::game_tables::game_tables()
    : surname_config_{ std::make_shared<surname_container_config>() },
      surname_weight_{},
      name_config_{ std::make_shared<name_container_config>() },
      name_weight_{},
      red_dot_{ std::make_shared<red_dot_container_config>() },
      item_{ std::make_shared<item_container_config>() },
      develop_{ std::make_shared<develop_container_config>() }
{
}

celeritas::game_tables::const_surname_container_config_shared_ptr celeritas::game_tables::get_surname_config() const
{
    return surname_config_;
}

void celeritas::game_tables::set_surname_config(const const_surname_container_config_shared_ptr& surname_config)
{
    surname_config_ = surname_config;

    surname_weight_.clear();

    for (const auto& [id, surname] : surname_config_->get_container())
    {
        surname_weight_.add_element(id, surname->get_weight());
    }
}

celeritas::game_tables::const_name_container_shared_ptr celeritas::game_tables::get_name_config() const
{
    return name_config_;
}

void celeritas::game_tables::set_name_config(const const_name_container_shared_ptr& name_config)
{
    name_config_ = name_config;

    name_weight_.clear();

    auto& null_name_weight_ = name_weight_[sex_type::null];
    auto& male_name_weight = name_weight_[sex_type::male];
    auto& female_name_weight = name_weight_[sex_type::female];

    for (const auto& [id, name] : name_config_->get_container())
    {
        null_name_weight_.add_element(id, name->get_weight());
        if (name->get_sex_type() != sex_type::female)
        {
            male_name_weight.add_element(id, name->get_weight());
        }
        if (name->get_sex_type() != sex_type::male)
        {
            female_name_weight.add_element(id, name->get_weight());
        }
    }
}

std::string celeritas::game_tables::get_surname() const
{
    const auto index = random_helper::get_random_index_by_weight(surname_weight_.get_weights());

    const auto id = surname_weight_.get_id(index);

    const auto surname = surname_config_->get(id);

    return (*surname)->get_name();
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

    const auto name = name_config_->get(id);

    return (*name)->get_name();
}

celeritas::game_tables::const_red_dot_container_shared_ptr celeritas::game_tables::get_red_dot_config() const
{
    return red_dot_;
}

void celeritas::game_tables::set_red_dot_config(const const_red_dot_container_shared_ptr& red_dot)
{
    red_dot_ = red_dot;
}

celeritas::game_tables::const_item_container_shared_ptr celeritas::game_tables::get_item_config() const
{
    return item_;
}

void celeritas::game_tables::set_item_config(const const_item_container_shared_ptr& item)
{
    item_ = item;
}

celeritas::game_tables::const_develop_container_shared_ptr celeritas::game_tables::get_develop_config() const
{
    return develop_;
}

void celeritas::game_tables::set_develop_config(const const_develop_container_shared_ptr& develop)
{
    develop_ = develop;
}