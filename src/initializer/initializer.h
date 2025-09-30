#pragma once

#include "initializer_factory.h"

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
        using resource_loader_unique_ptr = initializer_factory::resource_loader_unique_ptr;
        using path_type = boost::filesystem::path;
        using io_context_type = boost::asio::io_context;
        using executor_type = boost::asio::io_context::executor_type;
        using executor_work_guard_type = boost::asio::executor_work_guard<executor_type>;
        using signal_set_type = boost::asio::signal_set;

        [[nodiscard]] std::string get_server_type() const;

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
        io_context_type& io_context_;
        executor_work_guard_type work_guard_;

        // 新增一个信号集成员变量
        signal_set_type signals_;
    };
}
