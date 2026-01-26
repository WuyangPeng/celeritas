#pragma once

#include "common/core/session_route.h"
#include "config/config_fwd.h"
#include "message/message_fwd.h"
#include "service_registry/service_registry_fwd.h"

#include <boost/asio.hpp>
#include <google/protobuf/message.h>

#include <memory>

namespace celeritas
{
    class resource_loader_base : public std::enable_shared_from_this<resource_loader_base>
    {
    public:
        using class_type = resource_loader_base;
        using io_context_type = boost::asio::io_context;
        using protobuf_message = google::protobuf::Message;
        using const_app_config_shared_ptr = std::shared_ptr<const app_config>;
        using health_check_level_awaitable_type = boost::asio::awaitable<health_check_level_type>;
        using service_info_container = std::map<std::string, service_info>;
        using any_io_executor = boost::asio::any_io_executor;

        resource_loader_base() noexcept = default;

        virtual ~resource_loader_base() noexcept = default;

        resource_loader_base(const resource_loader_base& rhs) noexcept = default;

        resource_loader_base& operator=(const resource_loader_base& rhs) noexcept = default;

        resource_loader_base(resource_loader_base&& rhs) noexcept = default;

        resource_loader_base& operator=(resource_loader_base&& rhs) noexcept = default;

        [[nodiscard]] virtual std::string_view get_server_type() const = 0;

        virtual void process_check_tcp_clients_by_duration(const any_io_executor& any_io_executor) = 0;

        virtual void process_service_registry_by_duration() = 0;

        [[nodiscard]] virtual const_app_config_shared_ptr get_app_config() const = 0;

        [[nodiscard]] virtual health_check_level_awaitable_type get_health_check_level() = 0;

        [[nodiscard]] virtual bool write_to_server(const std::string& server_type, const header& header, const protobuf_message& request) = 0;

        [[nodiscard]] virtual bool write_to_server(const std::string& server_type, const std::string& instance_id, const header& header_message, const protobuf_message& request) = 0;

        [[nodiscard]] virtual bool write_to_client(const header& header, const protobuf_message& response) = 0;

        [[nodiscard]] virtual bool write_to_user(const std::string& server_type, int64_t session_id, const header& header, const protobuf_message& message) = 0;

        virtual void add_session_route(int64_t user_id, const session_route& session_route) = 0;

        virtual void check_client(const any_io_executor& any_io_executor, const std::string& server_type, const service_info_container& container) = 0;
    };
}