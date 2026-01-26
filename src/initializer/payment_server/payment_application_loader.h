#pragma once

#include "initializer/loader_base/application_loader.h"

namespace celeritas
{
    class payment_application_loader final : public application_loader
    {
    public:
        using class_type = payment_application_loader;
        using base_type = application_loader;

        explicit payment_application_loader(std::string_view server_type, app_config_shared_ptr app_config);

    private:
        void service_initialize_application() override;

        void register_message_handler();
    };
}


