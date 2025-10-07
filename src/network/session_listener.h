#pragma once

#include "listener.h"
#include "session.h"

#include <map>

namespace celeritas
{
    // 这个类需要删除
    class session_listener : public listener
    {
    public:
        using class_type = session_listener;
        using base_type = listener;

        session_listener(io_context_type& io_context,
                         network_message_callback_weak_ptr callback,
                         std::string game_server_id);

        void remove_session(int64_t session_id) override;

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