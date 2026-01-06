#include "http_handle_parameter_fixture.h"
#include "message/parameters/http_handle_parameter.h"

celeritas::http_handle_parameter_fixture::http_handle_parameter_fixture()
    : io_context_{},
      session_{ std::make_shared<mock_parameters_session>(io_context_) },
      resource_loader_{ std::make_shared<mock_parameters_resource_loader>() },
      application_loader_{ std::make_shared<mock_parameters_application_loader>() },
      parameter_{ std::make_shared<http_handle_parameter>("/test", "", session_, resource_loader_, application_loader_) }
{
}

celeritas::http_handle_parameter_fixture::io_context_type& celeritas::http_handle_parameter_fixture::get_io_context()
{
    return io_context_;
}

celeritas::http_handle_parameter_fixture::mock_parameters_session_shared_ptr celeritas::http_handle_parameter_fixture::get_session() const
{
    return session_;
}

celeritas::http_handle_parameter_fixture::mock_parameters_resource_loader_shared_ptr celeritas::http_handle_parameter_fixture::get_resource_loader() const
{
    return resource_loader_;
}

celeritas::http_handle_parameter_fixture::mock_parameters_application_loader_shared_ptr celeritas::http_handle_parameter_fixture::get_application_loader() const
{
    return application_loader_;
}

celeritas::http_handle_parameter_fixture::http_handle_parameter_shared_ptr celeritas::http_handle_parameter_fixture::get_parameter() const
{
    return parameter_;
}