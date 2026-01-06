#pragma once

#include "boost/asio/io_context.hpp"
#include "message/parameters/mock/mock_parameters_application_loader.h"
#include "message/parameters/mock/mock_parameters_resource_loader.h"
#include "message/parameters/mock/mock_parameters_session.h"

namespace celeritas
{
    class http_handle_parameter_fixture
    {
    public:
        using class_type = http_handle_parameter_fixture;
        using io_context_type = boost::asio::io_context;
        using mock_parameters_session_shared_ptr = std::shared_ptr<mock_parameters_session>;
        using mock_parameters_resource_loader_shared_ptr = std::shared_ptr<mock_parameters_resource_loader>;
        using mock_parameters_application_loader_shared_ptr = std::shared_ptr<mock_parameters_application_loader>;
        using http_handle_parameter_shared_ptr = std::shared_ptr<http_handle_parameter>;

        http_handle_parameter_fixture();

        [[nodiscard]] io_context_type& get_io_context();

        [[nodiscard]] mock_parameters_session_shared_ptr get_session() const;

        [[nodiscard]] mock_parameters_resource_loader_shared_ptr get_resource_loader() const;

        [[nodiscard]] mock_parameters_application_loader_shared_ptr get_application_loader() const;

        [[nodiscard]] http_handle_parameter_shared_ptr get_parameter() const;

    private:
        io_context_type io_context_;
        mock_parameters_session_shared_ptr session_;
        mock_parameters_resource_loader_shared_ptr resource_loader_;
        mock_parameters_application_loader_shared_ptr application_loader_;
        http_handle_parameter_shared_ptr parameter_;
    };
}