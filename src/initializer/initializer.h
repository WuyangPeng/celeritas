#pragma once

#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>
#include <memory>
#include <string>

namespace celeritas
{
    class initializer
    {
    public:
        using class_type = initializer;
        using initializer_unique_ptr = std::unique_ptr<initializer>;

        explicit initializer(std::string_view config_file_path, boost::asio::io_context& io_context) noexcept;

        virtual ~initializer() noexcept = default;

        initializer(const initializer& rhs) noexcept = default;

        initializer& operator=(const initializer& rhs) noexcept = default;

        initializer(initializer&& rhs) noexcept = default;

        initializer& operator=(initializer&& rhs) noexcept = default;

        void initialize();

        void run();

        [[nodiscard]] static initializer_unique_ptr create_initializer(const std::string_view& server_type, const std::string_view& config_file_path, boost::asio::io_context& io_context);

    private:
        virtual void initialize_config() = 0;

        virtual void initialize_resource() = 0;

        virtual void initialize_application() = 0;

        // 新增一个私有函数用于设置信号处理
        void setup_signal_handler();

        std::string config_file_path_;
        boost::asio::io_context& io_context_;
        boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guard_;

        // 新增一个信号集成员变量
        boost::asio::signal_set signals_;
    };
}
