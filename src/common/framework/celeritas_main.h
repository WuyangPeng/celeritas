#pragma once

#include "common/common_fwd.h"

#include <string>

namespace celeritas
{
    class celeritas_main
    {
    public:
        using class_type = celeritas_main;

        explicit celeritas_main(const std::string_view& server_type);

        virtual ~celeritas_main() noexcept = default;

        celeritas_main(const celeritas_main& rhs) noexcept = default;

        celeritas_main& operator=(const celeritas_main& rhs) noexcept = default;

        celeritas_main(celeritas_main&& rhs) noexcept = default;

        celeritas_main& operator=(celeritas_main&& rhs) noexcept = default;

        virtual void run(int argc, char** argv);

    protected:
        [[nodiscard]] std::string get_server_type() const;

    private:
        virtual void create_initializer(const command_line_config& command_line_config) = 0;

        std::string server_type_;
    };
}