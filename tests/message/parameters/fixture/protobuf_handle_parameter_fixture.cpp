#include "protobuf_handle_parameter_fixture.h"
#include "message/parameters/protobuf_handle_parameter.h"

#include <google/protobuf/empty.pb.h>

celeritas::protobuf_handle_parameter_fixture::protobuf_handle_parameter_fixture()
    : io_context_{},
      header_{ 123, 456 },
      request_message_{ std::make_shared<google::protobuf::Empty>() },
      session_{ std::make_shared<mock_parameters_session>(io_context_) },
      resource_loader_{ std::make_shared<mock_parameters_resource_loader>() },
      application_loader_{ std::make_shared<mock_parameters_application_loader>() },
      parameter_{ std::make_shared<protobuf_handle_parameter>(header_, request_message_, session_, resource_loader_, application_loader_) }
{
}

celeritas::protobuf_handle_parameter_fixture::io_context_type& celeritas::protobuf_handle_parameter_fixture::get_io_context()
{
    return io_context_;
}

celeritas::protobuf_handle_parameter_fixture::mock_parameters_session_shared_ptr celeritas::protobuf_handle_parameter_fixture::get_session() const
{
    return session_;
}

celeritas::protobuf_handle_parameter_fixture::mock_parameters_resource_loader_shared_ptr celeritas::protobuf_handle_parameter_fixture::get_resource_loader() const
{
    return resource_loader_;
}

celeritas::protobuf_handle_parameter_fixture::mock_parameters_application_loader_shared_ptr celeritas::protobuf_handle_parameter_fixture::get_application_loader() const
{
    return application_loader_;
}

celeritas::protobuf_handle_parameter_fixture::protobuf_handle_parameter_shared_ptr celeritas::protobuf_handle_parameter_fixture::get_parameter() const
{
    return parameter_;
}
