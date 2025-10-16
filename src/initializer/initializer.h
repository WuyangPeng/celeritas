#pragma once

#include "initializer_factory.h"
#include "common/buffer_guard.h"
#include "network/message_header.h"
#include "network/network_message_callback.h"

#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/filesystem.hpp>
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

    private:
        using configuration_loader_unique_ptr = initializer_factory::configuration_loader_unique_ptr;
        using resource_loader_unique_ptr = initializer_factory::resource_loader_unique_ptr;
        using application_loader_unique_ptr = initializer_factory::application_loader_unique_ptr;
        using path_type = boost::filesystem::path;
        using io_context_type = boost::asio::io_context;
        using executor_type = boost::asio::io_context::executor_type;
        using executor_work_guard_type = boost::asio::executor_work_guard<executor_type>;
        using signal_set_type = boost::asio::signal_set;

        [[nodiscard]] std::string get_server_type() const;

        [[nodiscard]] header get_header(const message_header& message_header, const buffer_guard& buffer_guard) const;

        void initialize_default_logger();

        void initialize_config();

        void initialize_resource();

        void initialize_application();

        // 设置信号处理
        void setup_signal_handler();

        std::string server_type_;
        path_type current_path_;
        configuration_loader_unique_ptr configuration_loader_;
        resource_loader_unique_ptr resource_loader_;
        application_loader_unique_ptr application_loader_;
        io_context_type io_context_;
        executor_work_guard_type work_guard_;

        // 新增一个信号集成员变量
        signal_set_type signals_;
    };
}
