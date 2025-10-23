#pragma once

#include "config/app_config.h"
#include "message/header.h"
#include "network/listener.h"
#include "network/network_message_callback.h"
#include "proto/response.pb.h"
#include "proto/request.pb.h"

namespace celeritas
{
    class resource_loader : public std::enable_shared_from_this<resource_loader>
    {
    public:
        using class_type = resource_loader;
        using app_config_shared_ptr = std::shared_ptr<app_config>;
        using network_message_callback_weak_ptr = std::weak_ptr<network_message_callback>;

        explicit resource_loader(app_config_shared_ptr app_config);

        virtual ~resource_loader() noexcept = default;

        resource_loader(const resource_loader& rhs) noexcept = delete;

        resource_loader& operator=(const resource_loader& rhs) noexcept = delete;

        resource_loader(resource_loader&& rhs) noexcept = delete;

        resource_loader& operator=(resource_loader&& rhs) noexcept = delete;

        void initialize(boost::asio::io_context& io_context, const network_message_callback_weak_ptr& network_message_callback);

        void release_resource();

        [[nodiscard]] bool write(const std::string& server_type, const header& header, const google::protobuf::Message& request) const;

    private:
        using listener_shared_ptr = std::shared_ptr<listener>;
        using listener_container_type = std::vector<listener_shared_ptr>;
        using tcp_client_shared_ptr = std::shared_ptr<tcp_client>;
        using tcp_client_container_type = std::vector<tcp_client_shared_ptr>;
        using steady_timer_type = boost::asio::steady_timer;
        using steady_timer_unique_ptr = std::unique_ptr<steady_timer_type>;
        using self_shared_ptr = std::shared_ptr<resource_loader>;
        using error_code_type = boost::system::error_code;

        void initialize_logger_resource();

        void initialize_database_resource(boost::asio::io_context& io_context);

        void initialize_server_resource(boost::asio::io_context& io_context, const network_message_callback_weak_ptr& network_message_callback);

        void initialize_health_check_url_resource();

        void initialize_service_registry_resource(boost::asio::io_context& io_context, const network_message_callback_weak_ptr& network_message_callback);

        void modify_service_registry_resource(boost::asio::io_context& io_context, const network_message_callback_weak_ptr& network_message_callback, int index);

        virtual void service_initialize_resource() = 0;

        void start_check_tcp_clients_timer(boost::asio::io_context& io_context);

        void start_check_tcp_clients_timer(boost::asio::io_context& io_context, const self_shared_ptr& self);

        void check_tcp_clients(boost::asio::io_context& io_context, const error_code_type& error_code);

        void process_check_tcp_clients(boost::asio::io_context& io_context);

        void process_check_tcp_clients_by_duration(boost::asio::io_context& io_context);

        void start_service_registry_timer(boost::asio::io_context& io_context);

        void start_service_registry_timer(boost::asio::io_context& io_context, const self_shared_ptr& self);

        void service_registry(boost::asio::io_context& io_context, const error_code_type& error_code);

        void process_service_registry();

        void process_service_registry_by_duration();

        app_config_shared_ptr app_config_;
        listener_container_type listener_;
        tcp_client_container_type tcp_clients_;
        bool is_service_registry_;
        steady_timer_unique_ptr check_tcp_clients_timer_interval_;
        steady_timer_unique_ptr service_registry_timer_interval_;
    };
}