#pragma once

#include <boost/log/sources/severity_logger.hpp>

namespace celeritas
{
    // 定义日志级别
    enum severity_level
    {
        debug,
        info,
        warning,
        error,
        fatal
    };

    // 全局日志对象
    // 在你的代码中，使用 LOG(severity_level) << "你的日志信息" 来记录
    #define LOG(level) BOOST_LOG_SEV(Logger::get(), level)

    class logger
    {
    public:
        using class_type = logger;
        using severity_logger_type = boost::log::sources::severity_logger<severity_level>;

        // 初始化日志系统
        static void init(const std::string& log_file_name);

        // 获取日志实例
        static severity_logger_type& get();

    private:
        static severity_logger_type logger_;
    };
}
