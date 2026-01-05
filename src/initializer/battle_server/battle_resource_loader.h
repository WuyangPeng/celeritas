#pragma once

#include "initializer/resource_loader.h"

namespace celeritas
{
    class battle_resource_loader final : public resource_loader
    {
    public:
        using class_type = battle_resource_loader;
        using base_type = resource_loader;

        explicit battle_resource_loader(std::string_view server_type, const_app_config_shared_ptr app_config);

    private:
        void service_initialize_resource(const any_io_executor& any_io_executor, const network_message_callback_weak_ptr& network_message_callback) override;
    };
}