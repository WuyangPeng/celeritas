#pragma once

#include "network/listener/listener_sessions_base.h"

namespace celeritas
{
    class mock_listener_sessions : public listener_sessions_base
    {
    public:
        using class_type = mock_listener_sessions;
        using base_type = listener_sessions_base;

        void remove_session(int64_t session_id) override;

        [[nodiscard]] bool is_remove_session_called() const;

        [[nodiscard]] int64_t get_last_removed_session_id() const;

    private:
        bool remove_session_called_ = false;
        int64_t last_removed_session_id_ = 0;
    };
}
