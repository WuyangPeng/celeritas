#pragma once

#include "auth_service_base.h"

namespace celeritas
{
    class auth_login : public auth_service_base
    {
    public:
        using class_type = auth_login;
        using base_type = auth_service_base;

        explicit auth_login(http_handle_parameter handle_parameter);
    };
}