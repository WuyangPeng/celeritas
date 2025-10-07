#pragma once

#include "network/network_fwd.h"

#include <boost/asio/awaitable.hpp>
#include <memory>
#include <map>

namespace celeritas
{
    class listener_accept : public std::enable_shared_from_this<listener_accept>
    {
    public:
        using class_type = listener_accept;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using network_message_callback_weak_ptr = std::weak_ptr<network_message_callback>;

        listener_accept() noexcept = default;

        virtual ~listener_accept() noexcept = default;

        listener_accept(const listener_accept& rhs) = default;

        listener_accept& operator=(const listener_accept& rhs) = default;

        listener_accept(listener_accept&& rhs) noexcept = default;

        listener_accept& operator=(listener_accept&& rhs) noexcept = default;

        virtual void stop() = 0;

        [[nodiscard]] virtual void_awaitable_type accept_connections() = 0;

        void remove_session(int64_t session_id);

    protected:
        using session_shared_ptr = std::shared_ptr<session>;

        void set_stop();

        [[nodiscard]] bool is_running() const noexcept;

        [[nodiscard]] int64_t get_next_session_id() noexcept;

        void add_session(const session_shared_ptr& session);

    private:
        using session_type_container_type = std::map<int64_t, session_shared_ptr>;

        std::atomic<bool> is_running_;
        session_type_container_type sessions_;
        int64_t session_id_;
    };
}