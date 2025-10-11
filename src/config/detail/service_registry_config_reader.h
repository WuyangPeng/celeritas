#pragma once

#include "config/service_registry_config.h"

#include <vector>

namespace celeritas
{
    class service_registry_config_reader
    {
    public:
        using class_type = service_registry_config_reader;
        using service_registry_config_container = std::vector<service_registry_config>;

        explicit service_registry_config_reader(std::string filename);

        [[nodiscard]] service_registry_config_container get_service_registry_config_container() const;

    private:
        void load_config();

        std::string filename_;
        service_registry_config_container service_registry_;
    };
}
