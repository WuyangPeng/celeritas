#include "server_config_reader.h"
#include "config/config_fwd.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

celeritas::server_config celeritas::server_config_reader::load_config(const std::string& filename)
{
    boost::property_tree::ptree tree{};

    boost::property_tree::xml_parser::read_xml(filename, tree);

    const auto node = tree.get_child("server");

    // 必需配置项
    const auto service_name = node.get<std::string>("service_name");
    const auto host = node.get<std::string>("host");
    const auto port = node.get<int>("port");

    const auto game_server_id = node.get<std::string>("game_server_id", "");

    server_config server_config{ service_name, host, port, game_server_id };

    return server_config;
}