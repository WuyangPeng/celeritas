#include "service_registry_config_reader.h"
#include "config/config_fwd.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

celeritas::service_registry_config celeritas::service_registry_config_reader::load_config(const std::string& filename)
{
    boost::property_tree::ptree tree{};

    boost::property_tree::xml_parser::read_xml(filename, tree);

    const auto node = tree.get_child("service_registry");

    const auto host = node.get<std::string>("host", default_service_registry_host.data());
    const auto port = node.get("port", default_service_registry_port);

    service_registry_config service_registry_config{ host, port };

    return service_registry_config;
}