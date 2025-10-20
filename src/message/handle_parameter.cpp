#include "handle_parameter.h"
#include "network/session.h"

celeritas::handle_parameter::handle_parameter(const header& header, protobuf_message_shared_ptr request_message, session_shared_ptr session)
    : header_{ header }, request_message_{ std::move(request_message) }, session_{ std::move(session) }
{
}

void celeritas::handle_parameter::write(const proto::response& response) const
{
    session_->write(header_, response);
}