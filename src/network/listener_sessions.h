#pragma once

#include "network/network_fwd.h"

#include <boost/asio/awaitable.hpp>
#include <map>
#include <memory>

namespace celeritas
{
    class listener_sessions : public std::enable_shared_from_this<listener_sessions>
    {
    public:
        using class_type = listener_sessions;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using network_message_callback_weak_ptr = std::weak_ptr<network_message_callback>;

        listener_sessions() noexcept = default;

        virtual ~listener_sessions() noexcept = default;

        listener_sessions(const listener_sessions& rhs) = delete;

        listener_sessions& operator=(const listener_sessions& rhs) = delete;

        listener_sessions(listener_sessions&& rhs) noexcept = delete;

        listener_sessions& operator=(listener_sessions&& rhs) noexcept = delete;

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
        int64_t session_id_ = 0;
    };
}