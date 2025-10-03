#pragma once

#include "config/app_config.h"
#include "network/listener.h"
#include "network/network_message_callback.h"

#include <boost/asio/io_context.hpp>

namespace celeritas
{
    class resource_loader
    {
    public:
        using class_type = resource_loader;
        using app_config_shared_ptr = std::shared_ptr<app_config>;
        using network_message_callback_weak_ptr = std::weak_ptr<network_message_callback>;

        explicit resource_loader(const app_config_shared_ptr& app_config);

        virtual ~resource_loader() noexcept = default;

        resource_loader(const resource_loader& rhs) noexcept = delete;

        resource_loader& operator=(const resource_loader& rhs) noexcept = delete;

        resource_loader(resource_loader&& rhs) noexcept = delete;

        resource_loader& operator=(resource_loader&& rhs) noexcept = delete;

        void initialize(boost::asio::io_context& io_context, const network_message_callback_weak_ptr& network_message_callback);

        void release_resource();

    private:
        using listener_shared_ptr = std::shared_ptr<listener>;
        using listener_container_type = std::vector<listener_shared_ptr>;

        void initialize_logger_resource();

        void initialize_database_resource(boost::asio::io_context& io_context);

        void initialize_server_resource(boost::asio::io_context& io_context, const network_message_callback_weak_ptr& network_message_callback);

        void initialize_health_check_url_resource();

        void initialize_service_registry_resource();

        virtual void service_initialize_resource() = 0;

        app_config_shared_ptr app_config_;
        listener_container_type listener_;
    };
}