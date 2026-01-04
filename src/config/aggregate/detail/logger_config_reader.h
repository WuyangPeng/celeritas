#pragma once

#include "config/local/logger_config.h"
#include "config/local/logger_level_config.h"

#include <boost/property_tree/ptree.hpp>

#include <vector>

namespace celeritas
{
    class logger_config_reader
    {
    public:
        using class_type = logger_config_reader;
        using const_logger_level_shared_ptr = std::shared_ptr<const logger_level_config>;
        using const_logger_shared_ptr = std::shared_ptr<const logger_config>;
        using logger_container = std::vector<const_logger_shared_ptr>;
        using const_container_shared_ptr = std::shared_ptr<const logger_container>;

        explicit logger_config_reader(std::string filename);

        [[nodiscard]] const_logger_level_shared_ptr get_logger_level_config() const;

        [[nodiscard]] const_container_shared_ptr get_logger_config_container() const;

    private:
        using node_type = boost::property_tree::basic_ptree<std::string, std::string>;
        using logger_level_shared_ptr = std::shared_ptr<logger_level_config>;
        using container_shared_ptr = std::shared_ptr<logger_container>;

        void load_config();

        void load_node(const std::string& name, const node_type& node);

        void load_node(const node_type& node);

        std::string filename_;
        container_shared_ptr logger_;
        logger_level_shared_ptr logger_level_;
    };
}
