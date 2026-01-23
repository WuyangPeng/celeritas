#include "mock_network_message_callback.h"

void celeritas::mock_network_message_callback::call_back(const message_header& message_header, buffer_guard buffer_guard, const session_shared_ptr& session)
{
    called_message_header_ = true;
    last_message_header_ = message_header;
}

void celeritas::mock_network_message_callback::call_back(const std::string& path, const urls_params_view_type& params, const session_shared_ptr& session)
{
    called_url_params_ = true;
    last_path_ = path;
}

void celeritas::mock_network_message_callback::call_back(const std::string& path, const std::string& params, const session_shared_ptr& session)
{
    called_string_params_ = true;
    last_path_ = path;
    last_string_params_ = params;
}

void celeritas::mock_network_message_callback::send_offline_message(int64_t session_id)
{
    called_offline_ = true;
    last_session_id_ = session_id;
}

bool celeritas::mock_network_message_callback::was_message_header_called() const
{
    return called_message_header_;
}

bool celeritas::mock_network_message_callback::was_url_params_called() const
{
    return called_url_params_;
}

bool celeritas::mock_network_message_callback::was_string_params_called() const
{
    return called_string_params_;
}

bool celeritas::mock_network_message_callback::was_offline_called() const
{
    return called_offline_;
}

const celeritas::message_header& celeritas::mock_network_message_callback::get_last_message_header() const
{
    return last_message_header_;
}

const std::string& celeritas::mock_network_message_callback::get_last_path() const
{
    return last_path_;
}

const std::string& celeritas::mock_network_message_callback::get_last_string_params() const
{
    return last_string_params_;
}

int64_t celeritas::mock_network_message_callback::get_last_session_id() const
{
    return last_session_id_;
}
