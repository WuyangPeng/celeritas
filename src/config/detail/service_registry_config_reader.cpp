#include "service_registry_config_reader.h"
#include "config/config_fwd.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <utility>

celeritas::service_registry_config_reader::service_registry_config_reader(std::string filename)
    : filename_{ std::move(filename) }, service_registry_{}
{
    load_config();
}

celeritas::service_registry_config_reader::service_registry_config_container celeritas::service_registry_config_reader::get_service_registry_config_container() const
{
    return service_registry_;
}

void celeritas::service_registry_config_reader::load_config()
{
    boost::property_tree::ptree tree{};

    boost::property_tree::xml_parser::read_xml(filename_, tree);

    for (const auto& [name , node] : tree.get_child("service_registry"))
    {
        const auto server_name = node.get<std::string>("<xmlattr>.name");

        const auto host = node.get<std::string>("host", default_service_registry_host.data());
        const auto port = node.get("port", default_service_registry_port);

        service_registry_config service_registry_config{ server_name, host, port };

        service_registry_.emplace_back(service_registry_config);
    }
}

