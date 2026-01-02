#pragma once

#include "common/common_constant.h"
#include "detail/common_logger_internal_fwd.h"
#include "detail/logger_namespace.h"

#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/utility/manipulators/add_value.hpp>

#include <optional>

namespace celeritas
{
    class logger
    {
    public:
        using class_type = logger;
        using severity_logger_type = log_sources::severity_logger<severity_level_type>;
        using severity_logger_reference_type = std::reference_wrapper<severity_logger_type>;
        using severity_logger_optional_type = std::optional<severity_logger_reference_type>;

        // 初始化日志系统
        static void init_global(severity_level_type global_level);

        static void init_console(severity_level_type console_level);

        static void init_file(const std::string& channel_name,
                              const std::string& log_file_name,
                              severity_level_type file_level,
                              int rotation_size,
                              bool also_to_console);

        // 获取日志实例
        [[nodiscard]] static severity_logger_optional_type get(std::string_view channel_name, severity_level_type level);

        [[nodiscard]] static severity_logger_optional_type get_default(severity_level_type level);

    private:
        static logger_impl& get_logger_impl() noexcept;
    };
}

#define LOG_INTERNAL(logger_handle, level) \
    if (auto logger = logger_handle; !logger) \
        static_cast<void>(0); /* 短路: 日志关闭 */ \
    else if (constexpr auto location = std::source_location::current(); false) \
        static_cast<void>(1); /* 永远不会执行，仅用于声明 location */ \
    else \
        BOOST_LOG_STREAM_SEV(logger->get(), boost::log::trivial::severity_level::level) \
        << boost::log::add_value(celeritas::log_function.data(), location.function_name()) \
        << boost::log::add_value(celeritas::log_file.data(), location.file_name()) \
        << boost::log::add_value(celeritas::log_line.data(), location.line())

// 全局日志对象
// 在你的代码中，使用 LOG(severity_level) << "你的日志信息" 来记录
#define LOG(level) \
    LOG_INTERNAL(celeritas::logger::get_default(boost::log::trivial::severity_level::level), level)

// 在你的代码中，使用 LOG(channel,severity_level) << "你的日志信息" 来记录
#define LOG_CHANNEL(channel, level) \
    LOG_INTERNAL(celeritas::logger::get(channel,boost::log::trivial::severity_level::level), level)
