#pragma once

#include "config/aggregate/server_config.h"

#include <boost/property_tree/ptree.hpp>

namespace celeritas
{
    class server_config_reader
    {
    public:
        using class_type = server_config_reader;

        static server_config load_config(const std::string& filename);

    private:
        using node_type = boost::property_tree::basic_ptree<std::string, std::string>;

        [[nodiscard]] static server_network_config get_server_network_config(const node_type& node);
    };
}
