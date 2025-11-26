#include "database_config_reader.h"
#include "common/celeritas_error.h"
#include "config/config_fwd.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

celeritas::database_config_reader::database_config_container celeritas::database_config_reader::load_config(const std::string& filename)
{
    database_config_container container{};

    boost::property_tree::ptree tree{};

    boost::property_tree::xml_parser::read_xml(filename, tree);

    for (const auto& [name , node] : tree.get_child("databases"))
    {
        if (name == "database")
        {
            container.emplace_back(get_database_node(node));
        }
    }

    return container;
}

celeritas::database_config celeritas::database_config_reader::get_database_node(const node_type& node)
{
    // 必需配置项
    const auto name = node.get<std::string>("<xmlattr>.name");
    const auto database_name = node.get<std::string>("database_type");
    const auto database_type = get_database_type(database_name);
    const auto host = node.get<std::string>("host");
    const auto port = node.get<int>("port");

    // 可选配置项
    const auto user = node.get<std::string>("user", "");
    const auto password = node.get<std::string>("password", "");
    const auto db_name = node.get<std::string>("db_name", "");
    const auto min_connections = node.get<int>("min_connections", default_database_min_connections);
    const auto max_connections = node.get<int>("max_connections", default_database_max_connections);
    const auto timeout_seconds = node.get<int>("timeout_seconds", default_database_timeout_seconds);
    const auto expire_seconds = node.get<int>("expire_seconds", default_database_expire_seconds);
    if (database_type == database_type::redis && expire_seconds <= 0)
    {
        throw celeritas_error{ "redis expire seconds must be greater than 0." };
    }

    return database_config{ name, database_type, host, port, user, password, db_name, min_connections, max_connections, timeout_seconds, expire_seconds };
}