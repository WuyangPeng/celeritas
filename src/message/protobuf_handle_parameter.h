#pragma once

#include "header.h"
#include "common/common_fwd.h"
#include "config/config_fwd.h"

#include <boost/asio/io_context.hpp>
#include <google/protobuf/message.h>

namespace celeritas
{
    class protobuf_handle_parameter
    {
    public:
        using class_type = protobuf_handle_parameter;
        using io_context_type = boost::asio::io_context;
        using protobuf_message = google::protobuf::Message;
        using protobuf_message_shared_ptr = std::shared_ptr<protobuf_message>;
        using session_shared_ptr = std::shared_ptr<session>;
        using resource_loader_shared_ptr = std::shared_ptr<resource_loader_base>;
        using application_loader_shared_ptr = std::shared_ptr<application_loader_base>;
        using app_config_const_shared_ptr = std::shared_ptr<const app_config>;

        protobuf_handle_parameter(io_context_type& io_context,
                                  const header& header,
                                  protobuf_message_shared_ptr request_message,
                                  const session_shared_ptr& session,
                                  const resource_loader_shared_ptr& resource_loader,
                                  const application_loader_shared_ptr& application_loader);

        void write(const protobuf_message& response) const;

        void write(const header& header, const protobuf_message& response) const;

        void write_to_server(const std::string& server_type, const protobuf_message& message) const;

        void write_to_server(const std::string& server_type, const std::string& instance_id, const protobuf_message& message) const;

        void write_to_client(const protobuf_message& response) const;

        [[nodiscard]] protobuf_message_shared_ptr get_protobuf_message() const;

        [[nodiscard]] io_context_type& get_io_context() const;

        [[nodiscard]] application_loader_shared_ptr get_application_loader() const;

        [[nodiscard]] int32_t get_rpc() const;

        [[nodiscard]] int64_t get_session_id() const;

        [[nodiscard]] app_config_const_shared_ptr get_app_config() const;

        [[nodiscard]] resource_loader_shared_ptr get_resource_loader() const;

        [[nodiscard]] server_network_type get_server_network_type() const;

    private:
        using session_weak_ptr = std::weak_ptr<session>;
        using resource_loader_weak_ptr = std::weak_ptr<resource_loader_base>;
        using application_loader_weak_ptr = std::weak_ptr<application_loader_base>;

        io_context_type& io_context_;
        header header_;
        protobuf_message_shared_ptr request_message_;
        session_weak_ptr session_;
        resource_loader_weak_ptr resource_loader_;
        application_loader_weak_ptr application_loader_;
    };
}
