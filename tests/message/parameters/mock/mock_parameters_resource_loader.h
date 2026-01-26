#pragma once

#include "common/framework/resource_loader_base.h"

namespace celeritas
{
    class mock_parameters_resource_loader final : public resource_loader_base
    {
    public:
        using class_type = mock_parameters_resource_loader;
        using base_type = resource_loader_base;

        mock_parameters_resource_loader();

        [[nodiscard]] std::string_view get_server_type() const override;

        void process_check_tcp_clients_by_duration(const any_io_executor& any_io_executor) override;

        void process_service_registry_by_duration() override;

        [[nodiscard]] const_app_config_shared_ptr get_app_config() const override;

        [[nodiscard]] health_check_level_awaitable_type get_health_check_level() override;

        [[nodiscard]] bool write_to_server(const std::string& server_type, const header& header, const protobuf_message& request) override;

        [[nodiscard]] bool write_to_server(const std::string& server_type, const std::string& instance_id, const header& header_message, const protobuf_message& request) override;

        [[nodiscard]] bool write_to_client(const header& header, const protobuf_message& response) override;

        [[nodiscard]] bool write_to_user(const std::string& server_type, int64_t session_id, const header& header, const protobuf_message& message) override;

        void add_session_route(int64_t user_id, const session_route& session_route) override;

        void check_client(const any_io_executor& any_io_executor, const std::string& server_type, const service_info_container& container) override;

        [[nodiscard]] int get_write_to_server_count() const;

        [[nodiscard]] std::string get_last_write_to_server_type() const;

        [[nodiscard]] std::string get_last_write_to_server_instance_id() const;

        [[nodiscard]] int get_write_to_user_count() const;

        [[nodiscard]] std::string get_last_write_to_user_server_type() const;

        [[nodiscard]] int get_write_to_client_count() const;

        [[nodiscard]] int get_check_client_count() const;

        [[nodiscard]] std::string get_last_check_client_server_type() const;

    private:
        [[nodiscard]] static const_app_config_shared_ptr create_app_config();

        const_app_config_shared_ptr app_config_;

        int write_to_server_count_ = 0;
        std::string last_write_to_server_type_;
        std::string last_write_to_server_instance_id_;
        int write_to_user_count_ = 0;
        std::string last_write_to_user_server_type_;
        int write_to_client_count_ = 0;
        int check_client_count_ = 0;
        std::string last_check_client_server_type_;
    };
}
