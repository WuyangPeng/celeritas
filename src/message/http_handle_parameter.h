#pragma once

#include "header.h"
#include "common/common_fwd.h"
#include "config/app_config.h"
#include "handler/handler_fwd.h"
#include "initializer/initializer_fwd.h"
#include "message/message_fwd.h"
#include "network/network_fwd.h"

#include <boost/asio.hpp>
#include <boost/url.hpp>

namespace celeritas
{
    class http_handle_parameter
    {
    public:
        using class_type = http_handle_parameter;
        using session_shared_ptr = std::shared_ptr<session>;
        using resource_loader_shared_ptr = std::shared_ptr<resource_loader_base>;
        using urls_params_view_type = boost::urls::params_view;
        using app_config_shared_ptr = std::shared_ptr<const app_config>;
        using io_context_type = boost::asio::io_context;
        using health_check_level_awaitable_type = boost::asio::awaitable<health_check_level_type>;

        http_handle_parameter(io_context_type& io_context, std::string path, const urls_params_view_type& params, const session_shared_ptr& session, const resource_loader_shared_ptr& resource_loader);

        http_handle_parameter(io_context_type& io_context, std::string path, std::string params, const session_shared_ptr& session, const resource_loader_shared_ptr& resource_loader);

        ~http_handle_parameter() noexcept = default;

        http_handle_parameter(const http_handle_parameter& rhs);

        http_handle_parameter& operator=(const http_handle_parameter& rhs) = delete;

        http_handle_parameter(http_handle_parameter&& rhs) noexcept;

        http_handle_parameter& operator=(http_handle_parameter&& rhs) noexcept = delete;

        [[nodiscard]] std::string get_path() const;

        [[nodiscard]] std::string get_response() const;

        void write(const std::string& response) const;

        [[nodiscard]] app_config_shared_ptr get_app_config() const;

        [[nodiscard]] health_check_level_awaitable_type get_health_check_level() const;

        [[nodiscard]] io_context_type& get_io_context() const;

    private:
        using session_weak_ptr = std::weak_ptr<session>;
        using resource_loader_weak_ptr = std::weak_ptr<resource_loader_base>;

        io_context_type& io_context_;
        std::string path_;
        urls_params_view_type params_;
        std::string response_;
        session_weak_ptr session_;
        resource_loader_weak_ptr resource_loader_;
    };
}
