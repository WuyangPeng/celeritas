#pragma once

#include "message/parameters/http_handle_parameter.h"
#include "message/basic/http_response.h"

#include <boost/asio/awaitable.hpp>

namespace celeritas
{
    class auth_service_base
    {
    public:
        using class_type = auth_service_base;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using any_io_executor = boost::asio::any_io_executor;
        using io_context_type = boost::asio::io_context;
        using http_handle_parameter_shared_ptr = std::shared_ptr<http_handle_parameter>;

        explicit auth_service_base(http_handle_parameter_shared_ptr handle_parameter);

        virtual ~auth_service_base() noexcept = default;

        auth_service_base(const auth_service_base& rhs) = default;

        auth_service_base& operator=(const auth_service_base& rhs) = delete;

        auth_service_base(auth_service_base&& rhs) noexcept = default;

        auth_service_base& operator=(auth_service_base&& rhs) noexcept = delete;

        [[nodiscard]] virtual void_awaitable_type response() = 0;

        [[nodiscard]] void_awaitable_type write_immediately(const http_response& response) const;

    protected:
        using optional_string = std::optional<std::string>;
        using app_config_const_shared_ptr = std::shared_ptr<const app_config>;
        using task_type = thread_safe_queue::task_type;

        [[nodiscard]] static std::string generate_token();

        [[nodiscard]] optional_string get_param(const std::string& key) const;

        [[nodiscard]] app_config_const_shared_ptr get_app_config() const;

        [[nodiscard]] http_handle_parameter_shared_ptr get_http_handle_parameter() const;

        void submit_task(task_type task) const;

        [[nodiscard]] any_io_executor get_any_io_executor() const;

    private:
        http_handle_parameter_shared_ptr handle_parameter_;
    };
}
