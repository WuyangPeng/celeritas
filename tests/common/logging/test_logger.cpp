#include "common/logging/logger.h"
#include "common/logging/detail/common_logger_internal_constant.h"
#include "config/config_fwd.h"
#include "detail/capture_clog.h"
#include "fixture/logger_fixture.h"

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/test/unit_test.hpp>

using namespace std::literals;

namespace
{
    constexpr std::string_view message_trace = "测试 trace 消息";
    constexpr std::string_view message_debug = "测试 debug 消息";
    constexpr std::string_view message_info = "测试 info 消息";
    constexpr std::string_view message_warning = "测试 warning 消息";
    constexpr std::string_view message_error = "测试 error 消息";
    constexpr std::string_view message_fatal = "测试 fatal 消息";
    constexpr std::string_view message_channel_info = "测试通道 info 消息";
    constexpr std::string_view message_file_only = "这条消息只应该出现在文件中，不应出现在控制台";

    void check_level(const celeritas::severity_level_type level, const bool expected)
    {
        const auto logger = celeritas::logger::get_default(level);
        BOOST_CHECK_EQUAL(logger.has_value(), expected);
    }

    void check_capture(const celeritas::capture_clog& capture)
    {
        const auto output = capture.str();
        BOOST_CHECK(output.find(message_trace) != std::string::npos);
        BOOST_CHECK(output.find(message_debug) != std::string::npos);
        BOOST_CHECK(output.find(message_info) != std::string::npos);
        BOOST_CHECK(output.find(message_warning) != std::string::npos);
        BOOST_CHECK(output.find(message_error) != std::string::npos);
        BOOST_CHECK(output.find(message_fatal) != std::string::npos);
    }
}

BOOST_FIXTURE_TEST_SUITE(logger_suite, celeritas::logger_fixture)

    BOOST_AUTO_TEST_CASE(test_logger_initialization_and_filtering)
    {
        // 初始化全局日志级别为 INFO
        celeritas::logger::init_global(boost::log::trivial::info);

        // 测试 get_default 获取满足级别的日志器 (INFO >= INFO)
        check_level(boost::log::trivial::info, true);

        // 测试 get_default 获取更高优先级的日志器 (WARNING >= INFO)
        check_level(boost::log::trivial::warning, true);

        // 测试 get_default 获取不足级别的日志器 (DEBUG < INFO)
        check_level(boost::log::trivial::debug, false);
    }

    BOOST_AUTO_TEST_CASE(test_console_initialization)
    {
        celeritas::logger::init_file(celeritas::default_channel.data(),
                                     celeritas::default_channel.data(),
                                     celeritas::severity_level_type::fatal,
                                     celeritas::default_logger_rotation_size,
                                     true);

        // 初始化控制台日志级别为 WARNING
        celeritas::logger::init_console(boost::log::trivial::warning);

        // 验证 WARNING 级别应该可用
        check_level(boost::log::trivial::warning, true);

        // 验证 INFO 级别应该被过滤 (INFO < WARNING)
        check_level(boost::log::trivial::info, false);
    }

    BOOST_AUTO_TEST_CASE(test_logger_macros)
    {
        const celeritas::capture_clog capture{};

        // 重新初始化为 TRACE 以允许所有日志
        celeritas::logger::init_global(boost::log::trivial::trace);
        celeritas::logger::init_console(boost::log::trivial::trace);

        // 这些宏应该能编译并运行无误
        LOG(trace) << message_trace;
        LOG(debug) << message_debug;
        LOG(info) << message_info;
        LOG(warning) << message_warning;
        LOG(error) << message_error;
        LOG(fatal) << message_fatal;

        boost::log::core::get()->flush();

        check_capture(capture);
    }

    BOOST_AUTO_TEST_CASE(test_channel_logger)
    {
        const celeritas::capture_clog capture{};

        // 确保全局级别允许 INFO
        celeritas::logger::init_global(boost::log::trivial::info);

        // 初始化一个特定的文件日志通道
        const std::string channel_name{ "test_channel" };
        const auto log_file = "test_channel"s + celeritas::log_daily_suffix.data() + celeritas::logger_extension.data();

        // 初始化文件日志，级别为 INFO，同时输出到控制台
        celeritas::logger::init_file(channel_name, log_file, boost::log::trivial::info, 1024 * 1024, true);

        // 测试记录到该通道
        LOG_CHANNEL(channel_name, info) << message_channel_info;

        boost::log::core::get()->flush();
        BOOST_CHECK(capture.str().find(message_channel_info) != std::string::npos);

        // 如果文件日志设置为 INFO，则 DEBUG 应该被过滤掉
        check_level(boost::log::trivial::debug, false);

        check_level(boost::log::trivial::info, true);
    }

    BOOST_AUTO_TEST_CASE(test_file_logger_no_console)
    {
        const celeritas::capture_clog capture{};

        celeritas::logger::init_console(boost::log::trivial::warning);

        const std::string channel_name{ "file_only_channel" };
        const auto log_file = "file_only"s + celeritas::log_daily_suffix.data() + celeritas::logger_extension.data();

        // 初始化仅文件日志，不输出到控制台，级别为 DEBUG
        celeritas::logger::init_file(channel_name, log_file, boost::log::trivial::debug, celeritas::default_logger_rotation_size, false);

        check_level(boost::log::trivial::debug, true);

        LOG_CHANNEL(channel_name, debug) << message_file_only;

        boost::log::core::get()->flush();
        BOOST_CHECK(capture.str().find(message_file_only) == std::string::npos);
    }

    BOOST_AUTO_TEST_CASE(test_unknown_channel)
    {
        // 测试获取一个未初始化的通道
        const std::string unknown_channel = "unknown_channel";

        // 尝试获取未知通道的 logger
        if (const auto channel_logger = celeritas::logger::get(unknown_channel, boost::log::trivial::info);
            channel_logger.has_value())
        {
            LOG_CHANNEL(unknown_channel, info) << "未知通道消息 (如果可见)";
        }

        // 只要不崩溃就算通过
        BOOST_CHECK(true);
    }

BOOST_AUTO_TEST_SUITE_END()
