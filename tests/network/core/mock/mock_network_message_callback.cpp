#include "mock_network_message_callback.h"

void celeritas::mock_network_message_callback::call_back(const message_header& message_header, buffer_guard buffer_guard, const session_shared_ptr& session)
{
}

void celeritas::mock_network_message_callback::call_back(const std::string& path, const urls_params_view_type& params, const session_shared_ptr& session)
{
}

void celeritas::mock_network_message_callback::call_back(const std::string& path, const std::string& params, const session_shared_ptr& session)
{
}

void celeritas::mock_network_message_callback::send_offline_message(int64_t session_id)
{
}