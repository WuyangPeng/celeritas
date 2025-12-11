#pragma once

#include "common/resource_loader_base.h"
#include "config/app_config.h"
#include "detail/gateway_mapping.h"
#include "detail/initializer_internal_fwd.h"
#include "message/message_fwd.h"
#include "network/listener.h"
#include "network/network_message_callback.h"
#include "proto/celeritas.pb.h"

#include <shared_mutex>

namespace celeritas
{
    class resource_loader : public resource_loader_base
    {
    public:
        using class_type = resource_loader;
        using base_type = resource_loader_base;
        using app_config_shared_ptr = std::shared_ptr<app_config>;
        using network_message_callback_weak_ptr = std::weak_ptr<network_message_callback>;

        using health_check_level_awaitable_type = boost::asio::awaitable<health_check_level_type>;

        explicit resource_loader(std::string_view server_type, app_config_shared_ptr app_config);

        ~resource_loader() noexcept override = default;

        resource_loader(const resource_loader& rhs) noexcept = delete;

        resource_loader& operator=(const resource_loader& rhs) noexcept = delete;

        resource_loader(resource_loader&& rhs) noexcept = delete;

        resource_loader& operator=(resource_loader&& rhs) noexcept = delete;

        [[nodiscard]] std::string_view get_server_type() const override;

        void initialize(io_context_type& io_context, const network_message_callback_weak_ptr& network_message_callback);

        void release_resource();

        [[nodiscard]] bool write(const std::string& server_type, const header& header, const protobuf_message& request) override;

        [[nodiscard]] bool write(const std::string& server_type, const std::string& instance_id, const header& header, const protobuf_message& request) override;

        [[nodiscard]] bool write_to_client(const header& header, const protobuf_message& response) override;

        void process_check_tcp_clients_by_duration(io_context_type& io_context) override;

        void process_service_registry_by_duration() override;

        [[nodiscard]] app_config_shared_ptr get_app_config() const override;

        [[nodiscard]] health_check_level_awaitable_type get_health_check_level() override;

        void add_gateway_mapping(int64_t user_id, gateway_mapping gateway_mapping);

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
        using gateway_mapping_type = std::map<int64_t, gateway_mapping>;

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
        std::string_view server_type_;
        gateway_mapping_type gateway_mapping_;
        std::shared_mutex mutex_;
    };
}