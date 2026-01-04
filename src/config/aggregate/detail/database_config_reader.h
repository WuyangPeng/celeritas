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
        using const_database_shared_ptr = std::shared_ptr<const database_config>;
        using database_container = std::vector<const_database_shared_ptr>;
        using const_container_shared_ptr = std::shared_ptr<const database_container>;

        [[nodiscard]] static const_container_shared_ptr load_config(const std::string& filename);

    private:
        using node_type = boost::property_tree::basic_ptree<std::string, std::string>;

        [[nodiscard]] static const_database_shared_ptr get_database_node(const node_type& node);
    };
}
