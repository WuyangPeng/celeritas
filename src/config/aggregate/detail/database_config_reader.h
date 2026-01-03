#pragma once

#include "config/local/database_config.h"

#include <boost/property_tree/ptree.hpp>

#include <vector>

namespace celeritas
{
    class database_config_reader
    {
    public:
        using class_type = database_config_reader;
        using database_config_container = std::vector<database_config>;

        [[nodiscard]] static database_config_container load_config(const std::string& filename);

    private:
        using node_type = boost::property_tree::basic_ptree<std::string, std::string>;

        [[nodiscard]] static database_config get_database_node(const node_type& node);
    };
}
