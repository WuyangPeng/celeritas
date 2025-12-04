#include "global_config_reader.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

celeritas::global_config celeritas::global_config_reader::load_config(const std::string& filename)
{
    boost::property_tree::ptree tree{};

    boost::property_tree::xml_parser::read_xml(filename, tree);

    const auto node = tree.get_child("global");

    // 必需配置项
    const auto external_host = node.get<std::string>("external_host");

    return global_config{ external_host };
}