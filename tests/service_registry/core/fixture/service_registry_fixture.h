#pragma once

namespace celeritas
{
    class service_registry_fixture
    {
    public:
        using class_type = service_registry_fixture;

        ~service_registry_fixture();

    private:
        void clear_services();
    };
}