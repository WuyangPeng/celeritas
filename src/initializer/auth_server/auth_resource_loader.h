#pragma once

#include "initializer/resource_loader.h"

namespace celeritas
{
    class auth_resource_loader final : public resource_loader
    {
    public:
        using class_type = auth_resource_loader;
        using base_type = resource_loader;

        explicit auth_resource_loader(std::string_view server_type, app_config_shared_ptr app_config);

    private:
        void service_initialize_resource(io_context_type& io_context, const network_message_callback_weak_ptr& network_message_callback) override;
    };
}
