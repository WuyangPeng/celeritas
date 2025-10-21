#pragma once

#include <string>

namespace celeritas
{
    class daemon
    {
    public:
        daemon();

        ~daemon();

        daemon(const daemon& rhs) = delete;

        daemon& operator=(const daemon& rhs) = delete;

        daemon(daemon&& rhs) = delete;

        daemon& operator=(daemon&& rhs) = delete;
    };
}