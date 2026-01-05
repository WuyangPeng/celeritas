#pragma once

#include "common/common_fwd.h"
#include "common/worker_pool/thread_safe_queue.h"
#include "config/config_fwd.h"
#include "config/local/database_config.h"
#include "service_registry/service_registry_fwd.h"

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
        using application_loader_shared_ptr = std::shared_ptr<application_loader_base>;
        using urls_params_view_type = boost::urls::params_view;
        using const_app_config_shared_ptr = std::shared_ptr<const app_config>;
        using any_io_executor = boost::asio::any_io_executor;
        using void_waitable_type = boost::asio::awaitable<void>;
        using health_check_level_awaitable_type = boost::asio::awaitable<health_check_level_type>;
        using optional_string = std::optional<std::string>;
        using task_type = thread_safe_queue::task_type;
        using const_database_config_shared_ptr = std::shared_ptr<const database_config>;

        http_handle_parameter(std::string path,
                              const urls_params_view_type& params,
                              const session_shared_ptr& session,
                              const resource_loader_shared_ptr& resource_loader,
                              const application_loader_shared_ptr& application_loader);

        http_handle_parameter(std::string path,
                              std::string params,
                              const session_shared_ptr& session,
                              const resource_loader_shared_ptr& resource_loader,
                              const application_loader_shared_ptr& application_loader);

        ~http_handle_parameter() noexcept = default;

        http_handle_parameter(const http_handle_parameter& rhs);

        http_handle_parameter& operator=(const http_handle_parameter& rhs) = delete;

        http_handle_parameter(http_handle_parameter&& rhs) noexcept;

        http_handle_parameter& operator=(http_handle_parameter&& rhs) noexcept = delete;

        [[nodiscard]] std::string get_path() const;

        [[nodiscard]] std::string get_response() const;

        [[nodiscard]] optional_string get_param(const std::string& key) const;

        [[nodiscard]] void_waitable_type write_immediately(const std::string& response) const;

        [[nodiscard]] const_app_config_shared_ptr get_app_config() const;

        [[nodiscard]] health_check_level_awaitable_type get_health_check_level() const;

        [[nodiscard]] any_io_executor get_any_io_executor() const;

        [[nodiscard]] std::string_view get_server_type() const;

        [[nodiscard]] const_database_config_shared_ptr get_database_config(const std::string& db_name) const;

        void submit_task(task_type task) const;

    private:
        using session_weak_ptr = std::weak_ptr<session>;
        using resource_loader_weak_ptr = std::weak_ptr<resource_loader_base>;
        using application_loader_weak_ptr = std::weak_ptr<application_loader_base>;
        using const_resource_loader_shared_ptr = std::shared_ptr<const resource_loader_base>;

        [[nodiscard]] const_resource_loader_shared_ptr get_resource_loader() const;

        [[nodiscard]] session_shared_ptr get_session() const;

        std::string path_;
        urls_params_view_type params_;
        std::string response_;
        session_weak_ptr session_;
        resource_loader_weak_ptr resource_loader_;
        application_loader_weak_ptr application_loader_;
    };
}
