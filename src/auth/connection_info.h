#pragma once

#include "config/server_network_type.h"

#include <boost/json.hpp>

#include <string>

namespace celeritas
{
    class connection_info
    {
    public:
        using class_type = connection_info;
        using json_value = boost::json::value;

        connection_info() noexcept = default;

        connection_info(std::string host, int port, server_network_type server_network);

        [[nodiscard]] std::string get_host() const;

        void set_host(const std::string& host);

        [[nodiscard]] int get_port() const;

        void set_port(int port);

        [[nodiscard]] server_network_type get_server_network() const;

        void set_server_network(server_network_type serverNetwork);

        static constexpr std::string_view host_description = "host";
        static constexpr std::string_view port_description = "port";
        static constexpr std::string_view server_network_description = "server_network";

    private:
        std::string host_;
        int port_ = 0;
        server_network_type server_network_ = server_network_type::unknown;
    };

    using connection_info_tag = boost::json::value_to_tag<connection_info>;

    [[nodiscard]] connection_info tag_invoke(connection_info_tag, const connection_info::json_value& value);

    void tag_invoke(boost::json::value_from_tag, connection_info::json_value& value, const connection_info& connection_info);
}