#pragma once

#include "player_state.h"

#include <boost/polymorphic_pointer_cast.hpp>

template <typename T>
std::shared_ptr<T> celeritas::player_state::get_component() const
{
    const auto component = components_.at(T::get_player_component_type());

    return boost::polymorphic_pointer_cast<T>(component);
}