#pragma once

#include "config/logger_config.h"
#include "config/logger_level_config.h"

#include <boost/property_tree/ptree.hpp>
#include <vector>

namespace celeritas
{
    class logger_config_reader
    {
    public:
        using class_type = logger_config_reader;
        using logger_config_container = std::vector<logger_config>;

        explicit logger_config_reader(std::string filename);

        [[nodiscard]] logger_level_config get_logger_level_config() const;

        [[nodiscard]] logger_config_container get_logger_config_container() const;

    private:
        using node_type = boost::property_tree::basic_ptree<std::string, std::string>;

        void load_config();

        void load_node(const std::string& name, const node_type& node);

        void load_node(const node_type& node);

        std::string filename_;
        logger_config_container logger_;
        logger_level_config logger_level_;
    };
}
