#include "common/core/celeritas_error.h"
#include "common/core/noexcept_safe_call_and_log.h"

#include <boost/test/unit_test.hpp>

#include <atomic>
#include <stdexcept>
#include <string>

BOOST_AUTO_TEST_SUITE(noexcept_safe_call_and_log_suite)

    // 测试：可调用对象不抛出异常时，函数正常执行
    BOOST_AUTO_TEST_CASE(test_no_exception)
    {
        std::atomic called{ false };
        auto func = [&] {
            called = true;
        };

        // noexcept_safe_call_and_log 自身不应抛出异常
        BOOST_CHECK_NO_THROW(celeritas::noexcept_safe_call_and_log(func, "test_channel", "error message"));
        // 验证函数被调用
        BOOST_CHECK(called);
    }

    // 测试：可调用对象抛出 std::exception 派生异常时，函数捕获异常且自身不抛出
    BOOST_AUTO_TEST_CASE(test_std_exception)
    {
        std::atomic called{ false };
        auto func = [&] {
            called = true;
            throw std::runtime_error("runtime error from func");
        };

        // noexcept_safe_call_and_log 自身不应抛出异常
        BOOST_CHECK_NO_THROW(celeritas::noexcept_safe_call_and_log(func, "test_channel", "error message"));
        // 验证函数被调用
        BOOST_CHECK(called);
    }

    // 测试：可调用对象抛出 celeritas_error 异常时，函数捕获异常且自身不抛出
    BOOST_AUTO_TEST_CASE(test_celeritas_error)
    {
        std::atomic called{ false };
        auto func = [&] {
            called = true;
            throw celeritas::celeritas_error("celeritas error from func");
        };

        // noexcept_safe_call_and_log 自身不应抛出异常
        BOOST_CHECK_NO_THROW(celeritas::noexcept_safe_call_and_log(func, "test_channel", "error message"));
        // 验证函数被调用
        BOOST_CHECK(called);
    }

    // 测试：可调用对象抛出未知异常时，函数捕获异常且自身不抛出
    BOOST_AUTO_TEST_CASE(test_unknown_exception)
    {
        std::atomic called{ false };
        auto func = [&] {
            called = true;
            // 抛出非 std::exception 派生异常
            throw 123;
        };

        // noexcept_safe_call_and_log 自身不应抛出异常
        BOOST_CHECK_NO_THROW(celeritas::noexcept_safe_call_and_log(func, "test_channel", "error message"));
        // 验证函数被调用
        BOOST_CHECK(called);
    }

BOOST_AUTO_TEST_SUITE_END()
