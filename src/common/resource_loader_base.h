#pragma once

#include "session_route.h"
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
        using app_config_shared_ptr = std::shared_ptr<app_config>;
        using health_check_level_awaitable_type = boost::asio::awaitable<health_check_level_type>;
        using protobuf_message = google::protobuf::Message;
        using service_info_container = std::map<std::string, service_info>;

        resource_loader_base() noexcept = default;

        virtual ~resource_loader_base() noexcept = default;

        resource_loader_base(const resource_loader_base& rhs) noexcept = default;

        resource_loader_base& operator=(const resource_loader_base& rhs) noexcept = default;

        resource_loader_base(resource_loader_base&& rhs) noexcept = default;

        resource_loader_base& operator=(resource_loader_base&& rhs) noexcept = default;

        [[nodiscard]] virtual std::string_view get_server_type() const = 0;

        virtual void process_check_tcp_clients_by_duration(io_context_type& io_context) = 0;

        virtual void process_service_registry_by_duration() = 0;

        [[nodiscard]] virtual app_config_shared_ptr get_app_config() const = 0;

        [[nodiscard]] virtual health_check_level_awaitable_type get_health_check_level() = 0;

        [[nodiscard]] virtual bool write(const std::string& server_type, const header& header, const protobuf_message& request) = 0;

        [[nodiscard]] virtual bool write(const std::string& server_type, const std::string& instance_id, const header& header, const protobuf_message& request) = 0;

        [[nodiscard]] virtual bool write_to_client(const header& header, const protobuf_message& response) = 0;

        virtual void add_session_route(int64_t user_id, session_route session_route) = 0;

        virtual void check_client(io_context_type& io_context, const std::string& server_type, const service_info_container& container) = 0;
    };
}