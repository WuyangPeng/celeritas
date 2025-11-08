#pragma once

#include "common/common_fwd.h"
#include "detail/common_internal_fwd.h"

#include <boost/log/trivial.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/utility/manipulators/add_value.hpp>

namespace celeritas
{
    namespace log_sources = boost::log::sources;
    namespace log_trivial = boost::log::trivial;

    class logger
    {
    public:
        using class_type = logger;
        using severity_level_type = log_trivial::severity_level;
        using severity_logger_type = log_sources::severity_logger<severity_level_type>;

        // 初始化日志系统
        static void init_global(severity_level_type level);

        static void init_console(severity_level_type console_level);

        static void init_file(const std::string& channel_name,
                              const std::string& log_file_name,
                              severity_level_type file_level,
                              int rotation_size,
                              bool also_to_console);

        // 获取日志实例
        [[nodiscard]] static severity_logger_type& get(std::string_view channel_name);

        [[nodiscard]] static severity_logger_type& get();

    private:
        static logger_impl& get_logger_impl();
    };
}

#define GET_SOURCE_LOCATION_INFO \
    if (constexpr auto location = std::source_location::current(); false) ; else

// 全局日志对象
// 在你的代码中，使用 LOG(severity_level) << "你的日志信息" 来记录
#define LOG(level) \
    GET_SOURCE_LOCATION_INFO \
    BOOST_LOG_STREAM_SEV(celeritas::logger::get(), boost::log::trivial::severity_level::level) \
    << boost::log::add_value(celeritas::log_function.data(), location.function_name()) \
    << boost::log::add_value(celeritas::log_file.data(), location.file_name()) \
    << boost::log::add_value(celeritas::log_line.data(), location.line())

// 在你的代码中，使用 LOG(channel,severity_level) << "你的日志信息" 来记录
#define LOG_CHANNEL(channel, level) \
    GET_SOURCE_LOCATION_INFO \
    BOOST_LOG_STREAM_SEV(celeritas::logger::get(channel), boost::log::trivial::severity_level::level) \
    << boost::log::add_value(celeritas::log_function.data(), location.function_name()) \
    << boost::log::add_value(celeritas::log_file.data(), location.file_name()) \
    << boost::log::add_value(celeritas::log_line.data(), location.line())
