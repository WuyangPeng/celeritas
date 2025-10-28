#include "http_handle_parameter.h"

celeritas::http_handle_parameter::http_handle_parameter(const std::string& path, const urls_params_view_type& params, const session_shared_ptr& session, const resource_loader_shared_ptr& resource_loader)
    : path_{ path }, params_{ params }, session_{ session }, resource_loader_{ resource_loader }
{
}

std::string celeritas::http_handle_parameter::get_path() const
{
    return path_;
}