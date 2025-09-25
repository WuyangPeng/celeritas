#pragma once

#include "initializer_factory.h"
#include "config/app_config.h"

#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/filesystem.hpp>
#include <memory>
#include <string>

namespace celeritas
{
    class initializer
    {
    public:
        using class_type = initializer;
        using initializer_unique_ptr = std::unique_ptr<initializer>;

        initializer(const std::string_view& server_type, std::string_view config_file_path, boost::asio::io_context& io_context);

        virtual ~initializer() noexcept = default;

        initializer(const initializer& rhs) noexcept = delete;

        initializer& operator=(const initializer& rhs) noexcept = delete;

        initializer(initializer&& rhs) noexcept = delete;

        initializer& operator=(initializer&& rhs) noexcept = delete;

        void initialize();

        void run();

    private:
        using configuration_loader_unique_ptr = initializer_factory::configuration_loader_unique_ptr;

        void initialize_default_logger();

        void initialize_config();

        void initialize_resource();

        void initialize_logger_resource();

        void initialize_database_resource();

        void initialize_server_resource();

        void initialize_health_check_url_resource();

        void initialize_service_registry_resource();

        void initialize_application();

        // 设置信号处理
        void setup_signal_handler();

        std::string config_file_path_;
        boost::filesystem::path current_path_;
        configuration_loader_unique_ptr configuration_loader_;
        boost::asio::io_context& io_context_;
        boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guard_;

        // 新增一个信号集成员变量
        boost::asio::signal_set signals_;
    };
}
