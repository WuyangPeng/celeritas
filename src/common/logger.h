#pragma once

#include "detail/common_internal_fwd.h"

#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/trivial.hpp>

namespace celeritas
{
    class logger
    {
    public:
        using class_type = logger;
        using severity_level_type = boost::log::trivial::severity_level;
        using severity_logger_type = boost::log::sources::severity_logger<severity_level_type>;

        // 初始化日志系统
        static void init_global(severity_level_type level);
        static void init_console(severity_level_type console_level);
        static void init_file(const std::string_view& channel_name,
                              const std::string_view& log_file_name,
                              severity_level_type file_level,
                              int rotation_size,
                              bool also_to_console);

        // 获取日志实例
        [[nodiscard]] static severity_logger_type& get(const std::string_view& channel_name);
        [[nodiscard]] static severity_logger_type& get();

    private:
        static logger_impl& get_logger_impl();
    };
}

// 全局日志对象
// 在你的代码中，使用 LOG(severity_level) << "你的日志信息" 来记录
#define LOG(level) BOOST_LOG_SEV(celeritas::logger::get(), boost::log::trivial::severity_level::level)
#define LOG_CHANNEL(channel, level) BOOST_LOG_SEV(celeritas::logger::get(channel), boost::log::trivial::severity_level::level)