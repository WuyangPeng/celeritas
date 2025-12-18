#pragma once

#include "container_config.h"
#include "common/celeritas_error.h"

#include <boost/core/demangle.hpp>

template <typename Element>
celeritas::container_config<Element>::optional_config celeritas::container_config<Element>::get(int id) const
{
    if (const auto iter = container_.find(id);
        iter != container_.cend())
    {
        return iter->second;
    }

    return std::nullopt;
}

template <typename Element>
void celeritas::container_config<Element>::add_config(const const_config_shared_ptr& config)
{
    if (const auto result = container_.emplace(config.get_id(), config);
        !result.second)
    {
        throw celeritas_error{ "config {} key is repeat,id = {}", boost::core::demangle(typeid(Element).name()), config.get_id() };
    }
}