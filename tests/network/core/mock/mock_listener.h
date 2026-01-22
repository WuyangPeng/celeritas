#pragma once

#include "network/core/listener.h"

namespace celeritas
{
    class mock_listener final : public listener
    {
    public:
        using class_type = mock_listener;
        using base_type = listener;

        mock_listener(const any_io_executor& any_io_executor,
                      network_message_callback_weak_ptr callback,
                      std::string game_server_id,
                      server_network_type server_network_type);

        [[nodiscard]] std::string test_get_game_server_id() const;

        [[nodiscard]] network_message_callback_weak_ptr test_get_network_message_callback();

        void stop() override;

        [[nodiscard]] void_awaitable_type accept_connections() override;

        [[nodiscard]] session_shared_ptr get_session(int64_t id) override;

        [[nodiscard]] bool write(const std::string& server_type, const std::string& instance_id, const header& header, const protobuf_message& request) override;
    };
}