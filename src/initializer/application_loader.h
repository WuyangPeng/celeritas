#pragma once

#include "boost/asio/io_context.hpp"
#include "config/app_config.h"
#include "initializer/initializer_fwd.h"
#include "message/protobuf_message_registry.h"
#include "common/common_fwd.h"
#include "worker_pool/worker_pool.h"

#include <boost/url.hpp>

namespace celeritas
{
    class application_loader
    {
    public:
        using class_type = application_loader;
        using app_config_shared_ptr = std::shared_ptr<app_config>;
        using base_message_handler_shared_ptr = std::shared_ptr<protobuf_base_message_handler>;
        using protobuf_message_shared_ptr = std::shared_ptr<google::protobuf::Message>;
        using message_registry_weak_ptr = std::weak_ptr<protobuf_message_registry>;
        using session_shared_ptr = std::shared_ptr<session>;
        using resource_loader_shared_ptr = std::shared_ptr<resource_loader_base>;
        using protobuf_message = google::protobuf::Message;
        using urls_params_view_type = boost::urls::params_view;
        using io_context_type = boost::asio::io_context;

        explicit application_loader(app_config_shared_ptr app_config);

        virtual ~application_loader() noexcept = default;

        application_loader(const application_loader& rhs) noexcept = delete;

        application_loader& operator=(const application_loader& rhs) noexcept = delete;

        application_loader(application_loader&& rhs) noexcept = delete;

        application_loader& operator=(application_loader&& rhs) noexcept = delete;

        void initialize();

        void stop();

        void register_handler(const base_message_handler_shared_ptr& handler);

        [[nodiscard]] bool dispatch(const header& header, const protobuf_message& current_message, const protobuf_message_shared_ptr& request_message, const session_shared_ptr& session, const resource_loader_shared_ptr& resource_loader);

        [[nodiscard]] bool dispatch(io_context_type& io_context, const std::string& path, const urls_params_view_type& params, const session_shared_ptr& session, const resource_loader_shared_ptr& resource_loader);

        [[nodiscard]] bool dispatch(io_context_type& io_context, const std::string& path, const std::string& params, const session_shared_ptr& session, const resource_loader_shared_ptr& resource_loader);

        [[nodiscard]] message_registry_weak_ptr get_message_registry();

    private:
        using worker_pool_unique_ptr = std::unique_ptr<worker_pool>;
        using message_registry_shared_ptr = std::shared_ptr<protobuf_message_registry>;
        using http_message_registry_shared_ptr = std::shared_ptr<http_message_registry>;

        void initialize_worker_pool();

        void initialize_message_registry();

        void initialize_health_check();

        virtual void service_initialize_application() = 0;

        app_config_shared_ptr app_config_;
        worker_pool_unique_ptr worker_pool_;
        message_registry_shared_ptr message_registry_;
        http_message_registry_shared_ptr http_message_registry_;
    };
}
