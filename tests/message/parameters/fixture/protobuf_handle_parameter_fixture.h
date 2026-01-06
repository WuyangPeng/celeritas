#pragma once

#include "message/parameters/protobuf_handle_parameter.h"
#include "message/parameters/mock/mock_parameters_application_loader.h"
#include "message/parameters/mock/mock_parameters_resource_loader.h"
#include "message/parameters/mock/mock_parameters_session.h"

#include <boost/asio/io_context.hpp>

namespace celeritas
{
    class protobuf_handle_parameter_fixture
    {
    public:
        using class_type = protobuf_handle_parameter_fixture;
        using io_context_type = boost::asio::io_context;
        using mock_parameters_session_shared_ptr = std::shared_ptr<mock_parameters_session>;
        using mock_parameters_resource_loader_shared_ptr = std::shared_ptr<mock_parameters_resource_loader>;
        using mock_parameters_application_loader_shared_ptr = std::shared_ptr<mock_parameters_application_loader>;
        using protobuf_handle_parameter_shared_ptr = std::shared_ptr<protobuf_handle_parameter>;
        using protobuf_message = google::protobuf::Message;
        using protobuf_message_shared_ptr = std::shared_ptr<protobuf_message>;

        protobuf_handle_parameter_fixture();

        [[nodiscard]] io_context_type& get_io_context();

        [[nodiscard]] mock_parameters_session_shared_ptr get_session() const;

        [[nodiscard]] mock_parameters_resource_loader_shared_ptr get_resource_loader() const;

        [[nodiscard]] mock_parameters_application_loader_shared_ptr get_application_loader() const;

        [[nodiscard]] protobuf_handle_parameter_shared_ptr get_parameter() const;

    private:
        io_context_type io_context_;
        header header_;
        protobuf_message_shared_ptr request_message_;
        mock_parameters_session_shared_ptr session_;
        mock_parameters_resource_loader_shared_ptr resource_loader_;
        mock_parameters_application_loader_shared_ptr application_loader_;
        std::shared_ptr<protobuf_handle_parameter> parameter_;
    };
}
