#include "http_handle_parameter.h"

#include <utility>
#include "network/session.h"

celeritas::http_handle_parameter::http_handle_parameter(std::string path, const urls_params_view_type& params, const session_shared_ptr& session, const resource_loader_shared_ptr& resource_loader)
    : path_{ std::move(path) }, params_{ params }, session_{ session }, resource_loader_{ resource_loader }
{
}

std::string celeritas::http_handle_parameter::get_path() const
{
    return path_;
}

void celeritas::http_handle_parameter::write(const std::string& response) const
{
    if (const auto session_shared_ptr = session_.lock();
        session_shared_ptr != nullptr)
    {
        session_shared_ptr->write(response);
    }
}