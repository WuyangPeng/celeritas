#include "common/command_line_config.tpp"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(command_line_config_suite)

    // 测试基本的命令行参数解析
    BOOST_AUTO_TEST_CASE(test_basic_parsing)
    {
        const char* argv[] = { "test_program", "--config_file_path=my_config.xml" };
        constexpr auto argc = std::size(argv);

        const celeritas::command_line_config config{ argc, const_cast<char**>(argv), "test_server" };

        BOOST_CHECK(!config.is_exit_requested());
        BOOST_CHECK_EQUAL(config.get<std::string>("config_file_path"), "my_config.xml");
    }

    // 测试请求帮助 (--help)
    BOOST_AUTO_TEST_CASE(test_help_request)
    {
        const char* argv[] = { "test_program", "--help" };
        constexpr auto argc = std::size(argv);

        const celeritas::command_line_config config{ argc, const_cast<char**>(argv), "test_server" };

        BOOST_CHECK(config.is_exit_requested());
    }

    // 测试请求帮助 (-h)
    BOOST_AUTO_TEST_CASE(test_help_short_request)
    {
        const char* argv[] = { "test_program", "-h" };
        constexpr auto argc = std::size(argv);

        const celeritas::command_line_config config{ argc, const_cast<char**>(argv), "test_server" };

        BOOST_CHECK(config.is_exit_requested());
    }

    // 测试带有自定义选项的构造函数
    BOOST_AUTO_TEST_CASE(test_with_custom_options)
    {
        celeritas::command_line_config::options_type custom_options{};
        custom_options["custom_param"] = "A custom string parameter.";
        custom_options["custom_int"] = "An integer parameter.";

        const char* argv[] = { "test_program", "--custom_param=hello", "--custom_int=42" };
        constexpr auto argc = std::size(argv);

        const celeritas::command_line_config config{ argc, const_cast<char**>(argv), "test_server", custom_options };

        BOOST_CHECK(!config.is_exit_requested());
        BOOST_CHECK_EQUAL(config.get<std::string>("custom_param"), "hello");
        BOOST_CHECK_EQUAL(config.get<std::string>("custom_int"), "42");
    }

    // 测试当参数未提供时的情况
    BOOST_AUTO_TEST_CASE(test_missing_parameter)
    {
        const char* argv[] = { "test_program" };
        constexpr auto argc = std::size(argv);

        const celeritas::command_line_config config{ argc, const_cast<char**>(argv), "test_server" };

        // Boost.program_options 会在尝试获取不存在的键时抛出异常
        BOOST_CHECK_THROW([config] { std::ignore = config.get<std::string>("non_existent_param"); }(), celeritas::celeritas_error);
    }

BOOST_AUTO_TEST_SUITE_END()
