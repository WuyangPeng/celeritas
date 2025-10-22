#pragma once

#include "common/common_fwd.h"

#include <string>

namespace celeritas
{
    class server_base
    {
    public:
        using class_type = server_base;

        explicit server_base(const std::string_view& server_type);

        virtual ~server_base() noexcept = default;

        server_base(const server_base& rhs) = default;

        server_base& operator=(const server_base& rhs) = default;

        server_base(server_base&& rhs) noexcept = default;

        server_base& operator=(server_base&& rhs) noexcept = default;

        void run(int argc, char** argv) const;

    private:
        void create_initializer(const command_line_config& command_line_config) const;

        std::string server_type_;
    };
}