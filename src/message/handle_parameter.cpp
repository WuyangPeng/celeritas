#include "handle_parameter.h"
#include "common/logger.h"
#include "common/resource_loader_base.h"
#include "common/session.h"

celeritas::handle_parameter::handle_parameter(const header& header, protobuf_message_shared_ptr request_message, const session_shared_ptr& session, const resource_loader_shared_ptr& resource_loader)
    : header_{ header }, request_message_{ std::move(request_message) }, session_{ session }, resource_loader_{ resource_loader }
{
}

void celeritas::handle_parameter::write(const protobuf_message& response) const
{
    if (const auto session_shared_ptr = session_.lock();
        session_shared_ptr != nullptr)
    {
        session_shared_ptr->write(header_, response);
    }
}

void celeritas::handle_parameter::write(const std::string& server_type, const protobuf_message& request) const
{
    if (const auto resource_loader_shared_ptr = resource_loader_.lock();
        resource_loader_shared_ptr != nullptr)
    {
        if (resource_loader_shared_ptr->write(server_type, header_, request))
        {
            LOG_CHANNEL(initializer_channel, trace) << "write message to server_type: " << server_type;
        }
    }
}

celeritas::handle_parameter::protobuf_message_shared_ptr celeritas::handle_parameter::get_protobuf_message() const
{
    return request_message_;
}