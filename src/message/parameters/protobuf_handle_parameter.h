#pragma once

#include "common/common_fwd.h"
#include "config/config_fwd.h"
#include "message/basic/header.h"
#include "service_registry/service_registry_fwd.h"

#include <boost/asio/io_context.hpp>
#include <google/protobuf/message.h>

namespace celeritas
{
    class protobuf_handle_parameter
    {
    public:
        using class_type = protobuf_handle_parameter;
        using any_io_executor = boost::asio::any_io_executor;
        using protobuf_message = google::protobuf::Message;
        using protobuf_message_shared_ptr = std::shared_ptr<protobuf_message>;
        using session_shared_ptr = std::shared_ptr<session>;
        using resource_loader_shared_ptr = std::shared_ptr<resource_loader_base>;
        using application_loader_shared_ptr = std::shared_ptr<application_loader_base>;
        using app_config_const_shared_ptr = std::shared_ptr<const app_config>;
        using service_info_container = std::map<std::string, service_info>;

        protobuf_handle_parameter(const header& header,
                                  protobuf_message_shared_ptr request_message,
                                  const session_shared_ptr& session,
                                  const resource_loader_shared_ptr& resource_loader,
                                  const application_loader_shared_ptr& application_loader);

        void write_to_response(const protobuf_message& response) const;

        void write_to_response(const header& header, const protobuf_message& response) const;

        void write_to_user(const std::string& server_type) const;

        void write_to_server(const std::string& server_type, const protobuf_message& message) const;

        void write_to_server(const std::string& server_type, const std::string& instance_id, const protobuf_message& message) const;

        void write_to_client(const protobuf_message& response) const;

        [[nodiscard]] int32_t get_rpc() const;

        [[nodiscard]] int64_t get_session_id() const;

        [[nodiscard]] server_network_type get_server_network_type() const;

        [[nodiscard]] int64_t get_user_id() const;

        [[nodiscard]] std::string get_instance_id() const;

        [[nodiscard]] protobuf_message_shared_ptr get_protobuf_message() const;

        [[nodiscard]] application_loader_shared_ptr get_application_loader() const;

        [[nodiscard]] resource_loader_shared_ptr get_resource_loader() const;

        [[nodiscard]] app_config_const_shared_ptr get_app_config() const;

        [[nodiscard]] any_io_executor get_any_io_executor() const;

        void check_client(const std::string& server_type, const service_info_container& container) const;

        void set_instance_id(const std::string& instance_id) const;

        void add_session_route(int64_t user_id, session_route session_route) const;

    private:
        using session_weak_ptr = std::weak_ptr<session>;
        using resource_loader_weak_ptr = std::weak_ptr<resource_loader_base>;
        using application_loader_weak_ptr = std::weak_ptr<application_loader_base>;

        [[nodiscard]] session_shared_ptr get_session() const;

        header header_;
        protobuf_message_shared_ptr request_message_;
        session_weak_ptr session_;
        resource_loader_weak_ptr resource_loader_;
        application_loader_weak_ptr application_loader_;
    };
}
