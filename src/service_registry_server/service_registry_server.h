#pragma once

namespace celeritas
{
    class service_registry_server
    {
    public:
        using class_type = service_registry_server;

        static void run(int argc, char** argv);
    };
}
