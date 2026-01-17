#pragma once

#include "daemon.h"
#include "initializer_factory.h"
#include "network/core/network_message_callback.h"

#include <boost/filesystem.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>

#include <memory>
#include <string>

namespace celeritas
{
    class initializer : public network_message_callback
    {
    public:
        using class_type = initializer;
        using base_type = network_message_callback;
        using initializer_shared_ptr = std::shared_ptr<initializer>;

        [[nodiscard]] static initializer_shared_ptr create(const std::string_view& server_type, std::string config_file_path);

        initializer(const std::string_view& server_type, std::string config_file_path);

        ~initializer() noexcept override = default;

        initializer(const initializer& rhs) noexcept = delete;

        initializer& operator=(const initializer& rhs) noexcept = delete;

        initializer(initializer&& rhs) noexcept = delete;

        initializer& operator=(initializer&& rhs) noexcept = delete;

        void initialize();

        void run();

        void call_back(const message_header& message_header, buffer_guard buffer_guard, const session_shared_ptr& session) override;

        void call_back(const std::string& path, const urls_params_view_type& params, const session_shared_ptr& session) override;

        void call_back(const std::string& path, const std::string& params, const session_shared_ptr& session) override;

        void send_offline_message(int64_t session_id) override;

    private:
        using configuration_loader_unique_ptr = initializer_factory::configuration_loader_unique_ptr;
        using resource_loader_shared_ptr = initializer_factory::resource_loader_shared_ptr;
        using application_loader_shared_ptr = initializer_factory::application_loader_shared_ptr;
        using path_type = boost::filesystem::path;
        using io_context_type = boost::asio::io_context;
        using executor_type = boost::asio::io_context::executor_type;
        using executor_work_guard_type = boost::asio::executor_work_guard<executor_type>;
        using signal_set_type = boost::asio::signal_set;
        using daemon_unique_ptr = std::unique_ptr<daemon>;

        [[nodiscard]] std::string get_server_type() const;

        [[nodiscard]] static header get_header(const message_header& message_header, const buffer_guard& buffer_guard);

        void initialize_default_logger();

        void initialize_config();

        void initialize_resource();

        void initialize_application();

        // 设置信号处理
        void setup_signal_handler();

        void stop();

        std::string server_type_;
        path_type current_path_;
        configuration_loader_unique_ptr configuration_loader_;
        resource_loader_shared_ptr resource_loader_;
        application_loader_shared_ptr application_loader_;
        io_context_type io_context_;
        executor_work_guard_type work_guard_;
        daemon_unique_ptr daemon_;

        // 信号集成员变量
        signal_set_type signals_;
    };
}
