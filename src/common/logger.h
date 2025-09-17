#pragma once

#include <boost/log/trivial.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

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
        static void init(const std::string& log_file_name, severity_level_type file_level);

        // 获取日志实例
        [[nodiscard]] static severity_logger_type& get();
    };
}

// 全局日志对象
// 在你的代码中，使用 LOG(severity_level) << "你的日志信息" 来记录
#define LOG(level) BOOST_LOG_SEV(logger::get(), boost::log::trivial::severity_level::level)