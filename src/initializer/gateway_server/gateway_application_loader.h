#pragma once

#include "initializer/loader_base/application_loader.h"

namespace celeritas
{
    class gateway_application_loader final : public application_loader
    {
    public:
        using class_type = gateway_application_loader;
        using base_type = application_loader;

        explicit gateway_application_loader(std::string_view server_type, app_config_shared_ptr app_config);

    private:
        void service_initialize_application() override;

        void register_message_handler();
    };
}


