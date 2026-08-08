#pragma once

#include "network/core/http_parameter.h"

namespace celeritas
{
    class auth_parameter : public http_parameter
    {
    public:
        using class_type = auth_parameter;
        using base_type = http_parameter;
        using base_type::base_type;

    protected:
        using base_type::optional_http_response;
        using base_type::optional_string;
    };
}
