#pragma once

#include "../initializer/server_base.h"

namespace celeritas
{
    class chat_server final : public server_base
    {
    public:
        using class_type = chat_server;
        using base_type = server_base;

        chat_server();
    };
}



