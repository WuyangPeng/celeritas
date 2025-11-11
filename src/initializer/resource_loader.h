#pragma once

#include "config/app_config.h"
#include "detail/initializer_internal_fwd.h"
#include "message/header.h"
#include "message/message_fwd.h"
#include "network/listener.h"
#include "network/network_message_callback.h"
#include "proto/celeritas.pb.h"

namespace celeritas
{
    class resource_loader : public std::enable_shared_from_this<resource_loader>
    {
    public:
        using class_type = resource_loader;
        using app_config_shared_ptr = std::shared_ptr<app_config>;
        using network_message_callback_weak_ptr = std::weak_ptr<network_message_callback>;
        using io_context_type = boost::asio::io_context;
        using protobuf_message = google::protobuf::Message;
        using health_check_level_awaitable_type = boost::asio::awaitable<health_check_level_type>;

        explicit resource_loader(app_config_shared_ptr app_config);

        virtual ~resource_loader() noexcept = default;

        resource_loader(const resource_loader& rhs) noexcept = delete;

        resource_loader& operator=(const resource_loader& rhs) noexcept = delete;

        resource_loader(resource_loader&& rhs) noexcept = delete;

        resource_loader& operator=(resource_loader&& rhs) noexcept = delete;

        void initialize(io_context_type& io_context, const network_message_callback_weak_ptr& network_message_callback);

        void release_resource();

        [[nodiscard]] bool write(const std::string& server_type, const header& header, const protobuf_message& request) const;

        void process_check_tcp_clients_by_duration(io_context_type& io_context);

        void process_service_registry_by_duration();

        [[nodiscard]] app_config_shared_ptr get_app_config() const;

        [[nodiscard]] health_check_level_awaitable_type get_health_check_level() const;

    protected:
        using tcp_client_shared_ptr = std::shared_ptr<tcp_client>;
        using tcp_client_container_type = std::vector<tcp_client_shared_ptr>;

    private:
        using listener_shared_ptr = std::shared_ptr<listener>;
        using listener_container_type = std::vector<listener_shared_ptr>;

        using steady_timer_type = boost::asio::steady_timer;
        using steady_timer_unique_ptr = std::unique_ptr<steady_timer_type>;
        using self_shared_ptr = std::shared_ptr<resource_loader>;
        using error_code_type = boost::system::error_code;
        using check_tcp_clients_timer_shared_ptr = std::shared_ptr<check_tcp_clients_timer>;
        using service_registry_timer_shared_ptr = std::shared_ptr<service_registry_timer>;
        using buffer_pool_timer_shared_ptr = std::shared_ptr<buffer_pool_timer>;
        using service_registry_config_container = std::map<std::string, service_registry_config>;

        void initialize_logger_resource();

        void initialize_database_resource(io_context_type& io_context);

        void initialize_server_resource(io_context_type& io_context, const network_message_callback_weak_ptr& network_message_callback);

        void initialize_service_registry_resource(io_context_type& io_context, const network_message_callback_weak_ptr& network_message_callback);

        void modify_service_registry_resource(io_context_type& io_context, const network_message_callback_weak_ptr& network_message_callback, int index);

        virtual void service_initialize_resource(io_context_type& io_context, const network_message_callback_weak_ptr& network_message_callback) = 0;

        void start_check_tcp_clients_timer(io_context_type& io_context);

        void start_service_registry_timer(io_context_type& io_context);

        void start_buffer_pool_timer(io_context_type& io_context);

        [[nodiscard]] tcp_client_shared_ptr get_random_client(io_context_type& io_context, const network_message_callback_weak_ptr& network_message_callback, const service_registry_config_container& service_registry) const;

        app_config_shared_ptr app_config_;
        listener_container_type listener_;
        tcp_client_container_type tcp_clients_;
        bool is_service_registry_;
        check_tcp_clients_timer_shared_ptr check_tcp_clients_timer_;
        service_registry_timer_shared_ptr service_registry_timer_;
        buffer_pool_timer_shared_ptr buffer_pool_timer_;
        int64_t start_server_time_;
    };
}