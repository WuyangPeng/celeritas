#include "protobuf_handle_parameter.h"
#include "common/celeritas_error.h"
#include "common/logger.h"
#include "common/resource_loader_base.h"
#include "common/session.h"

celeritas::protobuf_handle_parameter::protobuf_handle_parameter(io_context_type& io_context,
                                                                const header& header,
                                                                protobuf_message_shared_ptr request_message,
                                                                const session_shared_ptr& session,
                                                                const resource_loader_shared_ptr& resource_loader,
                                                                const application_loader_shared_ptr& application_loader)
    : io_context_{ io_context },
      header_{ header },
      request_message_{ std::move(request_message) },
      session_{ session },
      resource_loader_{ resource_loader },
      application_loader_{ application_loader }
{
}

void celeritas::protobuf_handle_parameter::write(const protobuf_message& response) const
{
    if (const auto session_shared_ptr = session_.lock();
        session_shared_ptr != nullptr)
    {
        session_shared_ptr->write(header_, response);
    }
}

void celeritas::protobuf_handle_parameter::write(const header& header, const protobuf_message& response) const
{
    if (const auto session_shared_ptr = session_.lock();
        session_shared_ptr != nullptr)
    {
        session_shared_ptr->write(header, response);
    }
}

void celeritas::protobuf_handle_parameter::write_to_server(const std::string& server_type, const protobuf_message& message) const
{
    if (const auto resource_loader_shared_ptr = resource_loader_.lock();
        resource_loader_shared_ptr != nullptr)
    {
        if (resource_loader_shared_ptr->write(server_type, header_, message))
        {
            LOG_CHANNEL(initializer_channel, trace) << "write message to server_type: " << server_type;
        }
    }
}

void celeritas::protobuf_handle_parameter::write_to_server(const std::string& server_type, const std::string& instance_id, const protobuf_message& message) const
{
    if (const auto resource_loader_shared_ptr = resource_loader_.lock();
        resource_loader_shared_ptr != nullptr)
    {
        if (resource_loader_shared_ptr->write(server_type, instance_id, header_, message))
        {
            LOG_CHANNEL(initializer_channel, trace) << "write message to server_type: " << server_type;
        }
    }
}

celeritas::protobuf_handle_parameter::protobuf_message_shared_ptr celeritas::protobuf_handle_parameter::get_protobuf_message() const
{
    return request_message_;
}

celeritas::protobuf_handle_parameter::io_context_type& celeritas::protobuf_handle_parameter::get_io_context() const
{
    return io_context_;
}

celeritas::protobuf_handle_parameter::application_loader_shared_ptr celeritas::protobuf_handle_parameter::get_application_loader() const
{
    if (const auto application_loader_shared_ptr = application_loader_.lock();
        application_loader_shared_ptr != nullptr)
    {
        return application_loader_shared_ptr;
    }

    throw celeritas_error{ "no application loader exists." };
}

int32_t celeritas::protobuf_handle_parameter::get_rpc() const
{
    return header_.get_rpc();
}