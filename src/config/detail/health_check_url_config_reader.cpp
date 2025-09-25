#include "health_check_url_config_reader.h"
#include "config/config_fwd.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

celeritas::health_check_url_config celeritas::health_check_url_config_reader::load_config(const std::string& filename)
{
    boost::property_tree::ptree tree{};

    boost::property_tree::xml_parser::read_xml(filename, tree);

    const auto node = tree.get_child("health_check_url");

    // 必需配置项
    const auto url = node.get<std::string>("url");

    const auto interval = node.get<int>("interval", default_health_check_interval);
    const auto timeout = node.get<int>("timeout", default_health_check_timeout);

    health_check_url_config health_check_url_config{ url, interval, timeout };

    return health_check_url_config;
}