#pragma once

#include "container_config.h"
#include "common/core_utilities/celeritas_error.h"

#include <boost/core/demangle.hpp>

template <typename Element, typename IndexType>
celeritas::container_config<Element, IndexType>::optional_config celeritas::container_config<Element, IndexType>::get(IndexType id) const
{
    if (const auto iter = container_.find(id);
        iter != container_.cend())
    {
        return iter->second;
    }

    return std::nullopt;
}

template <typename Element, typename IndexType>
void celeritas::container_config<Element, IndexType>::add_config(const const_config_shared_ptr& config)
{
    if (const auto result = container_.emplace(config->get_id(), config);
        !result.second)
    {
        throw celeritas_error{ "config {} key is repeat,id = {}", boost::core::demangle(typeid(Element).name()), static_cast<int>(config->get_id()) };
    }
}

template <typename Element, typename IndexType>
const celeritas::container_config<Element, IndexType>::container& celeritas::container_config<Element, IndexType>::get_container() const
{
    return container_;
}