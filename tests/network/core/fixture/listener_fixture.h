#pragma once

#include "config/basic/server_network_type.h"
#include "network/core/network_message_callback.h"
#include "network/core/mock/mock_listener.h"

#include <boost/asio/io_context.hpp>

#include <memory>

namespace celeritas
{
    class listener_fixture
    {
    public:
        using class_type = listener_fixture;
        using io_context_type = boost::asio::io_context;
        using mock_listener_shared_ptr = std::shared_ptr<mock_listener>;

        listener_fixture();

        [[nodiscard]] io_context_type& get_io_context();

        [[nodiscard]] const std::string& get_game_server_id() const;

        [[nodiscard]] server_network_type get_server_type() const;

        [[nodiscard]] mock_listener_shared_ptr get_listener();

    private:
        using network_message_callback_weak_ptr = std::weak_ptr<network_message_callback>;

        io_context_type io_context_;
        network_message_callback_weak_ptr callback_;
        std::string game_server_id_;
        server_network_type server_type_;
        mock_listener_shared_ptr listener_;
    };
}