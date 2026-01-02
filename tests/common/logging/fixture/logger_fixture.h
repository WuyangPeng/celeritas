#pragma once

namespace celeritas
{
    class logger_fixture
    {
    public:
        using class_type = logger_fixture;

        logger_fixture();

        static void reset();
    };
}