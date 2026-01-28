#include "protobuf_handle_parameter.h"
#include "common/core/celeritas_error.h"
#include "common/framework/resource_loader_base.h"
#include "common/framework/session.h"
#include "common/logging/logger.h"

celeritas::protobuf_handle_parameter::protobuf_handle_parameter(const header& header,
                                                                protobuf_message_shared_ptr request_message,
                                                                const session_shared_ptr& session,
                                                                const resource_loader_shared_ptr& resource_loader,
                                                                const application_loader_shared_ptr& application_loader)
    : header_{ header },
      request_message_{ std::move(request_message) },
      session_{ session },
      resource_loader_{ resource_loader },
      application_loader_{ application_loader }
{
}

void celeritas::protobuf_handle_parameter::write_to_response(const protobuf_message& response) const
{
    get_session()->write(header_, response);
}

void celeritas::protobuf_handle_parameter::write_to_response(const header& header, const protobuf_message& response) const
{
    get_session()->write(header, response);
}

void celeritas::protobuf_handle_parameter::write_to_user(const std::string& server_type) const
{
    if (get_resource_loader()->write_to_user(server_type, get_session_id(), header_, *request_message_))
    {
        LOG_CHANNEL(initializer_channel, trace) << "write message to server_type: " << server_type;
    }
}

void celeritas::protobuf_handle_parameter::write_to_server(const std::string& server_type, const protobuf_message& message) const
{
    if (get_resource_loader()->write_to_server(server_type, header_, message))
    {
        LOG_CHANNEL(initializer_channel, trace) << "write message to server_type: " << server_type;
    }
}

void celeritas::protobuf_handle_parameter::write_to_server(const std::string& server_type, const std::string& instance_id, const protobuf_message& message) const
{
    if (get_resource_loader()->write_to_server(server_type, instance_id, header_, message))
    {
        LOG_CHANNEL(initializer_channel, trace) << "write message to server type: " << server_type;
    }
}

void celeritas::protobuf_handle_parameter::write_to_client(const protobuf_message& response) const
{
    if (get_resource_loader()->write_to_client(header_, response))
    {
        LOG_CHANNEL(initializer_channel, trace) << "write message to client,user id = : " << header_.get_user_id();
    }
}

int32_t celeritas::protobuf_handle_parameter::get_rpc() const
{
    return header_.get_rpc();
}

int64_t celeritas::protobuf_handle_parameter::get_session_id() const
{
    return get_session()->get_session_id();
}

celeritas::server_network_type celeritas::protobuf_handle_parameter::get_server_network_type() const
{
    return get_session()->get_server_network_type();
}

int64_t celeritas::protobuf_handle_parameter::get_user_id() const
{
    return header_.get_user_id();
}

std::string celeritas::protobuf_handle_parameter::get_instance_id() const
{
    return get_session()->get_instance_id();
}

celeritas::protobuf_handle_parameter::protobuf_message_shared_ptr celeritas::protobuf_handle_parameter::get_protobuf_message() const
{
    return request_message_;
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

celeritas::protobuf_handle_parameter::resource_loader_shared_ptr celeritas::protobuf_handle_parameter::get_resource_loader() const
{
    if (const auto resource_loader_shared_ptr = resource_loader_.lock();
        resource_loader_shared_ptr != nullptr)
    {
        return resource_loader_shared_ptr;
    }

    throw celeritas_error{ "resource loader is null." };
}

celeritas::protobuf_handle_parameter::const_app_config_shared_ptr celeritas::protobuf_handle_parameter::get_app_config() const
{
    return get_resource_loader()->get_app_config();
}

celeritas::protobuf_handle_parameter::any_io_executor celeritas::protobuf_handle_parameter::get_any_io_executor() const
{
    return get_session()->get_any_io_executor();
}

void celeritas::protobuf_handle_parameter::check_client(const std::string& server_type, const service_info_container& container) const
{
    return get_resource_loader()->check_client(get_any_io_executor(), server_type, container);
}

void celeritas::protobuf_handle_parameter::set_instance_id(const std::string& instance_id) const
{
    return get_session()->set_instance_id(instance_id);
}

void celeritas::protobuf_handle_parameter::add_session_route(const int64_t user_id, session_route session_route) const
{
    get_resource_loader()->add_session_route(user_id, std::move(session_route));
}

celeritas::protobuf_handle_parameter::session_shared_ptr celeritas::protobuf_handle_parameter::get_session() const
{
    if (const auto session_shared_ptr = session_.lock();
        session_shared_ptr != nullptr)
    {
        return session_shared_ptr;
    }

    throw celeritas_error{ "session is close." };
}



