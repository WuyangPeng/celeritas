#pragma once

namespace celeritas
{
    class crash
    {
    public:
        using class_type = crash;

        static void set_signal();

        static void crash_handler(int signal_number);
    };
}