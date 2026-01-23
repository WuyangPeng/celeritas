#pragma once

#include "network/core/listener.h"
#include "network/core/session_base.h"

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

        [[nodiscard]] bool write(const std::string& server_type,
                                 const std::string& instance_id,
                                 const header& header,
                                 const protobuf_message& request) override;

        [[nodiscard]] bool stop_called() const;

        [[nodiscard]] bool accept_connections_called() const;

        void set_session_to_return(session_shared_ptr session);

        [[nodiscard]] bool write_called() const;

        [[nodiscard]] std::string get_write_server_type() const;

        [[nodiscard]] std::string get_write_instance_id() const;

        [[nodiscard]] header get_write_header() const;

        [[nodiscard]] const protobuf_message* get_write_request() const;

    private:
        bool stop_called_ = false;
        bool accept_connections_called_ = false;
        session_shared_ptr session_to_return_ = nullptr;
        bool write_called_ = false;
        std::string write_server_type_;
        std::string write_instance_id_;
        header write_header_;
        const protobuf_message* write_request_ = nullptr;
    };
}
