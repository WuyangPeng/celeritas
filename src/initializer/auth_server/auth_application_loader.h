#pragma once

#include "initializer/application_loader.h"

namespace celeritas
{
    class auth_application_loader final : public application_loader
    {
    public:
        using class_type = auth_application_loader;
        using base_type = application_loader;

        explicit auth_application_loader(std::string_view server_type, app_config_shared_ptr app_config);

    private:
        void service_initialize_application() override;

        void register_message_handler();
    };
}


