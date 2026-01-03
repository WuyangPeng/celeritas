#pragma once

#include "config/local/service_registry_config.h"

#include <boost/property_tree/ptree.hpp>

#include <vector>

namespace celeritas
{
    class service_registry_config_reader
    {
    public:
        using class_type = service_registry_config_reader;
        using registry_container = std::vector<service_registry_config>;
        using const_registry_container_shared_ptr = std::shared_ptr<const registry_container>;

        explicit service_registry_config_reader(std::string filename);

        [[nodiscard]] const_registry_container_shared_ptr get_service_registry_config_container() const;

    private:
        using node_type = boost::property_tree::basic_ptree<std::string, std::string>;
        using registry_container_shared_ptr = std::shared_ptr<registry_container>;

        void load_config();

        void load_node(const node_type& node);

        std::string filename_;
        registry_container_shared_ptr service_registry_;
    };
}
