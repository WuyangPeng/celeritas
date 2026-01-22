#pragma once

#include "listener_sessions_base.h"
#include "config/config_fwd.h"
#include "message/message_fwd.h"
#include "network/network_fwd.h"

#include <boost/asio/awaitable.hpp>
#include <google/protobuf/message.h>

#include <map>
#include <memory>

namespace celeritas
{
    class listener_sessions : public listener_sessions_base
    {
    public:
        using class_type = listener_sessions;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using network_message_callback_weak_ptr = std::weak_ptr<network_message_callback>;
        using session_shared_ptr = std::shared_ptr<session_base>;
        using protobuf_message = google::protobuf::Message;

        explicit listener_sessions(server_network_type server_network_type) noexcept;

        void remove_session(int64_t session_id) override;

        [[nodiscard]] session_shared_ptr get_session(int64_t id);

        [[nodiscard]] server_network_type get_server_network_type() const noexcept;

        [[nodiscard]] bool write(const std::string &instance_id, const celeritas::header &header, const protobuf_message &request);

    protected:
        void set_stop();

        [[nodiscard]] bool is_running() const noexcept;

        [[nodiscard]] int64_t get_next_session_id() noexcept;

        void add_session(const session_shared_ptr& session);

    private:
        using session_type_container_type = std::map<int64_t, session_shared_ptr>;

        std::atomic<bool> is_running_ = true;
        session_type_container_type sessions_;
        int64_t session_id_ = 0;
        server_network_type server_network_type_;
    };
}