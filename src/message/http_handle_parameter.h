#pragma once

#include "header.h"
#include "initializer/initializer_fwd.h"
#include "network/network_fwd.h"

#include <boost/url.hpp>

namespace celeritas
{
    class http_handle_parameter
    {
    public:
        using class_type = http_handle_parameter;
        using session_shared_ptr = std::shared_ptr<session>;
        using resource_loader_shared_ptr = std::shared_ptr<resource_loader>;
        using urls_params_view_type = boost::urls::params_view;

        http_handle_parameter(const std::string& path, const urls_params_view_type& params, const session_shared_ptr& session, const resource_loader_shared_ptr& resource_loader);

        [[nodiscard]] std::string get_path() const;

        void write(const std::string& response) const;

    private:
        using session_weak_ptr = std::weak_ptr<session>;
        using resource_loader_weak_ptr = std::weak_ptr<resource_loader>;

        std::string path_;
        urls_params_view_type params_;
        session_weak_ptr session_;
        resource_loader_weak_ptr resource_loader_;
    };
}
