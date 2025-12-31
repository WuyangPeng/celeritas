#pragma once

#include "common/framework/resource_loader_base.h"

namespace celeritas
{
    class mock_framework_resource_loader final : public resource_loader_base
    {
    public:
        using class_type = mock_framework_resource_loader;
        using base_type = resource_loader_base;

        mock_framework_resource_loader();

        [[nodiscard]] std::string_view get_server_type() const override;

        void process_check_tcp_clients_by_duration(io_context_type& io_context) override;

        void process_service_registry_by_duration() override;

        [[nodiscard]] app_config_shared_ptr get_app_config() const override;

        [[nodiscard]] health_check_level_awaitable_type get_health_check_level() override;

        [[nodiscard]] bool write_to_server(const std::string& server_type, const header& header, const protobuf_message& request) override;

        [[nodiscard]] bool write_to_server(const std::string& server_type, const std::string& instance_id, const header& header_message, const protobuf_message& request) override;

        [[nodiscard]] bool write_to_client(const header& header, const protobuf_message& response) override;

        [[nodiscard]] bool write_to_user(const std::string& server_type, int64_t session_id, const header& header, const protobuf_message& message) override;

        void add_session_route(int64_t user_id, session_route session_route) override;

        void check_client(io_context_type& io_context, const std::string& server_type, const service_info_container& container) override;
    };
}
