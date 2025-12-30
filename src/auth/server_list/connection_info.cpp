#include "connection_info.h"
#include "common/core_utilities/enum_cast.h"

celeritas::connection_info::connection_info(std::string host, const int port, const server_network_type server_network)
    : host_{ std::move(host) }, port_{ port }, server_network_{ server_network }
{
}

std::string celeritas::connection_info::get_host() const
{
    return host_;
}

void celeritas::connection_info::set_host(const std::string& host)
{
    host_ = host;
}

int celeritas::connection_info::get_port() const
{
    return port_;
}

void celeritas::connection_info::set_port(const int port)
{
    port_ = port;
}

celeritas::server_network_type celeritas::connection_info::get_server_network() const
{
    return server_network_;
}

void celeritas::connection_info::set_server_network(const server_network_type serverNetwork)
{
    server_network_ = serverNetwork;
}

celeritas::connection_info celeritas::tag_invoke(connection_info_tag, const connection_info::json_value& value)
{
    const auto& object = value.as_object();

    auto host = boost::json::value_to<std::string>(object.at(connection_info::host_description));
    const auto port = boost::json::value_to<int>(object.at(connection_info::port_description));
    const auto server_network = boost::json::value_to<int>(object.at(connection_info::server_network_description));

    return connection_info{ std::move(host), port, underlying_cast_enum<server_network_type>(server_network) };
}

void celeritas::tag_invoke(boost::json::value_from_tag, connection_info::json_value& value, const connection_info& connection_info)
{
    value = {
        { connection_info::host_description, connection_info.get_host() },
        { connection_info::port_description, connection_info.get_port() },
        { connection_info::server_network_description, enum_cast_underlying(connection_info.get_server_network()) }
    };
}