#pragma once

#include "config/config_constant.h"

#include <string>

namespace celeritas
{
    class service_registry_config
    {
    public:
        using class_type = service_registry_config;

        service_registry_config() noexcept = default;

        service_registry_config(std::string name, std::string host, int port) noexcept;

        [[nodiscard]] std::string get_host() const;

        [[nodiscard]] int get_port() const noexcept;

        [[nodiscard]] std::string get_name() const;

    private:
        std::string name_;
        std::string host_ = default_service_registry_host.data();
        int port_ = default_service_registry_port;
    };
}



