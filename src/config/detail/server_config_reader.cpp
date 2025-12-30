#include "server_config_reader.h"
#include "common/core_utilities/celeritas_error.h"
#include "common/common_fwd.h"
#include "config/config_fwd.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

celeritas::server_config celeritas::server_config_reader::load_config(const std::string& filename)
{
    boost::property_tree::ptree tree{};

    boost::property_tree::xml_parser::read_xml(filename, tree);

    const auto node = tree.get_child("server");

    // 必需配置项
    const auto instance_id = node.get<std::string>("instance_id");
    const auto service_name = node.get<std::string>("service_name");
    const auto host = node.get<std::string>("host");

    // 可选配置项
    const auto game_server_id = node.get<std::string>("game_server_id", "");
    const auto worker_pool = node.get<int>("worker_pool", default_worker_pool_size);
    const auto datacenter_id = node.get<int>("datacenter_id", default_datacenter_id);
    const auto worker_id = node.get<int>("worker_id", default_worker_id);
    const auto load_game_config = node.get<bool>("load_game_config", false);

    if (datacenter_id > max_datacenter_id || datacenter_id < 0)
    {
        throw celeritas_error{ "datacenter id can't be greater than {} or less than 0", max_datacenter_id };
    }

    if (worker_id > max_worker_id || worker_id < 0)
    {
        throw celeritas_error{ "worker id can't be greater than {} or less than 0", max_worker_id };
    }

    server_config::server_network_config_container_type container{};

    for (const auto& [name , element] : node)
    {
        if (name == "network")
        {
            container.emplace_back(get_server_network_config(element));
        }
    }

    return server_config{ instance_id, service_name, container, game_server_id, host, worker_pool, datacenter_id, worker_id, load_game_config };
}

celeritas::server_network_config celeritas::server_config_reader::get_server_network_config(const node_type& node)
{
    const auto network_node = node.get<std::string>("network_type");
    const auto network_type = get_server_network_type(network_node);

    const auto port = node.get<int>("port");

    return server_network_config{ network_type, port };
}