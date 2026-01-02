#pragma once

#include "common_logger_internal_constant.h"
#include "logger_namespace.h"
#include "common/common_constant.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

namespace celeritas
{
    class logger_support
    {
    public:
        using class_type = logger_support;
        using time_type = boost::posix_time::ptime;
        using thread_id_value_type = log_attributes::current_thread_id::value_type;

        [[nodiscard]] static auto get_formatter()
        {
            // 设置日志格式
            return log_expressions::stream
                   << "["
                   << log_expressions::format_date_time<time_type>("TimeStamp", log_date_time_format.data())
                   << "]["
                   << log_trivial::severity
                   << "]["
                   << log_expressions::attr<thread_id_value_type>("ThreadID")
                   << "]["
                   << log_expressions::attr<std::string>(log_file.data())
                   << ":"
                   << log_expressions::attr<uint_least32_t>(log_line.data())
                   << "]"
                   << log_expressions::smessage;
        }
    };
}