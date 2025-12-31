#pragma once

#include "common/framework/celeritas_main.h"
#include "common/common_fwd.h"

#include <string>

namespace celeritas
{
    class server_base : public celeritas_main
    {
    public:
        using class_type = server_base;
        using base_type = celeritas_main;

        explicit server_base(const std::string_view& server_type);

    private:
        void create_initializer(const command_line_config& command_line_config) override;
    };
}