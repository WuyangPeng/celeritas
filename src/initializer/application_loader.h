#pragma once

#include "boost/asio/io_context.hpp"
#include "config/app_config.h"
#include "initializer/initializer_fwd.h"
#include "message/protobuf_message_registry.h"
#include "common/common_fwd.h"
#include "../common/worker_pool.h"
#include "common/application_loader_base.h"

#include <boost/url.hpp>

namespace celeritas
{
    class application_loader : public application_loader_base
    {
    public:
        using class_type = application_loader;
        using base_type = application_loader_base;
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

        void initialize();

        void stop();

        void register_handler(const base_message_handler_shared_ptr& handler);

        [[nodiscard]] bool dispatch(io_context_type& io_context, const header& header, const protobuf_message& current_message, const protobuf_message_shared_ptr& request_message, const session_shared_ptr& session, const resource_loader_shared_ptr& resource_loader);

        [[nodiscard]] bool dispatch(io_context_type& io_context, const std::string& path, const urls_params_view_type& params, const session_shared_ptr& session, const resource_loader_shared_ptr& resource_loader);

        [[nodiscard]] bool dispatch(io_context_type& io_context, const std::string& path, const std::string& params, const session_shared_ptr& session, const resource_loader_shared_ptr& resource_loader);

        [[nodiscard]] message_registry_weak_ptr get_message_registry();

        void submit_task(task_type task) override;

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
