#include "common/logger/logger.h"
#include "config/config_fwd.h"

#include <boost/log/trivial.hpp>
#include <boost/test/unit_test.hpp>

using namespace std::literals;

namespace
{
    // 辅助 fixture，用于在每个测试用例前重置 logger 状态
    struct logger_fixture
    {
        logger_fixture()
        {
            reset();
        }

        static void reset()
        {
            // 由于 logger 是单例且没有提供显式的 reset 接口，
            // 我们通过重新初始化来模拟重置状态。
            celeritas::logger::init_global(boost::log::trivial::trace);
            celeritas::logger::init_console(boost::log::trivial::trace);
            celeritas::logger::init_file(celeritas::default_channel.data(),
                                         celeritas::default_channel.data(),
                                         celeritas::severity_level_type::trace,
                                         celeritas::default_logger_rotation_size,
                                         true);
        }
    };
}

BOOST_FIXTURE_TEST_SUITE(logger_suite, logger_fixture)

    BOOST_AUTO_TEST_CASE(test_logger_initialization_and_filtering)
    {
        // 初始化全局日志级别为 INFO
        celeritas::logger::init_global(boost::log::trivial::info);

        // 测试 get_default 获取满足级别的日志器 (INFO >= INFO)
        const auto logger_info = celeritas::logger::get_default(boost::log::trivial::info);
        BOOST_CHECK(logger_info.has_value());

        // 测试 get_default 获取更高优先级的日志器 (WARNING >= INFO)
        const auto logger_warning = celeritas::logger::get_default(boost::log::trivial::warning);
        BOOST_CHECK(logger_warning.has_value());

        // 测试 get_default 获取不足级别的日志器 (DEBUG < INFO)
        // 假设日志器实现会根据上面设置的全局级别进行过滤
        const auto logger_debug = celeritas::logger::get_default(boost::log::trivial::debug);
        BOOST_CHECK(!logger_debug.has_value());
    }

    BOOST_AUTO_TEST_CASE(test_console_initialization)
    {
        celeritas::logger::init_file(celeritas::default_channel.data(),
                                     celeritas::default_channel.data(),
                                     celeritas::severity_level_type::fatal,
                                     celeritas::default_logger_rotation_size,
                                     true);

        // 初始化控制台日志级别为 WARNING
        // 这通常会影响默认的控制台输出
        celeritas::logger::init_console(boost::log::trivial::warning);

        // 验证 WARNING 级别应该可用
        const auto logger_warning = celeritas::logger::get_default(boost::log::trivial::warning);
        BOOST_CHECK(logger_warning.has_value());

        // 验证 INFO 级别应该被过滤 (INFO < WARNING)
        // 假设 init_console 设置了控制台 sink 的过滤器
        const auto logger_info = celeritas::logger::get_default(boost::log::trivial::info);
        BOOST_CHECK(!logger_info.has_value());
    }

    BOOST_AUTO_TEST_CASE(test_logger_macros)
    {
        // 重新初始化为 TRACE 以允许所有日志
        celeritas::logger::init_global(boost::log::trivial::trace);
        celeritas::logger::init_console(boost::log::trivial::trace);

        // 这些宏应该能编译并运行无误
        LOG(trace) << "测试 trace 消息";
        LOG(debug) << "测试 debug 消息";
        LOG(info) << "测试 info 消息";
        LOG(warning) << "测试 warning 消息";
        LOG(error) << "测试 error 消息";
        LOG(fatal) << "测试 fatal 消息";
    }

    BOOST_AUTO_TEST_CASE(test_channel_logger)
    {
        // 确保全局级别允许 INFO
        celeritas::logger::init_global(boost::log::trivial::info);

        // 初始化一个特定的文件日志通道
        const std::string channel_name{ "test_channel" };
        const auto log_file = "test_channel"s + celeritas::log_daily_suffix.data() + celeritas::logger_extension.data();

        // 初始化文件日志，级别为 INFO，同时输出到控制台
        celeritas::logger::init_file(channel_name, log_file, boost::log::trivial::info, 1024 * 1024, true);

        // 测试记录到该通道
        LOG_CHANNEL(channel_name, info) << "测试通道 info 消息";

        // 测试通道上的过滤
        // 如果文件日志设置为 INFO，则 DEBUG 应该被过滤掉
        const auto logger_debug = celeritas::logger::get(channel_name, boost::log::trivial::debug);
        BOOST_CHECK(!logger_debug.has_value());

        const auto logger_info = celeritas::logger::get(channel_name, boost::log::trivial::info);
        BOOST_CHECK(logger_info.has_value());
    }

    BOOST_AUTO_TEST_CASE(test_file_logger_no_console)
    {
        const std::string channel_name{ "file_only_channel" };
        const auto log_file = "file_only"s + celeritas::log_daily_suffix.data() + celeritas::logger_extension.data();

        // 初始化仅文件日志，不输出到控制台，级别为 DEBUG
        celeritas::logger::init_file(channel_name, log_file, boost::log::trivial::debug, celeritas::default_logger_rotation_size, false);

        const auto logger = celeritas::logger::get(channel_name, boost::log::trivial::debug);
        BOOST_CHECK(logger.has_value());

        LOG_CHANNEL(channel_name, debug) << "这条消息只应该出现在文件中，不应出现在控制台";
    }

    BOOST_AUTO_TEST_CASE(test_unknown_channel)
    {
        // 测试获取一个未初始化的通道
        const std::string unknown_channel = "unknown_channel";

        // 尝试获取未知通道的 logger

        if (const auto logger = celeritas::logger::get(unknown_channel, boost::log::trivial::info);
            logger.has_value())
        {
            LOG_CHANNEL(unknown_channel, info) << "未知通道消息 (如果可见)";
        }

        // 只要不崩溃就算通过
        BOOST_CHECK(true);
    }

BOOST_AUTO_TEST_SUITE_END()
