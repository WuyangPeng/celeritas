#pragma once

#include "listener_sessions.h"

namespace celeritas
{
    class listener_accept : public listener_sessions
    {
    public:
        using class_type = listener_accept;
        using base_type = listener_sessions;

        virtual void stop() = 0;

        [[nodiscard]] virtual void_awaitable_type accept_connections() = 0;
    };
}