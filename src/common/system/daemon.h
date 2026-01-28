#pragma once

#include <string>

namespace celeritas
{
    class daemon
    {
    public:
        explicit daemon(std::string_view server_type);

        ~daemon() noexcept;

        daemon(const daemon& rhs) = delete;

        daemon& operator=(const daemon& rhs) = delete;

        daemon(daemon&& rhs) = delete;

        daemon& operator=(daemon&& rhs) = delete;

    private:
        void stop();

        std::string server_type_;
        std::string pid_file_name_;
    };
}