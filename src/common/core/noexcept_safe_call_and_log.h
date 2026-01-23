#pragma once

#include "common/logging/logger.h"

#include <boost/asio/awaitable.hpp>

namespace celeritas
{
    template <typename Func>
    void noexcept_safe_call_and_log(Func f, const std::string_view channel_name, const std::string& error_message) noexcept
    {
        try
        {
            f();
        }
        catch (const std::exception& exception)
        {
            try
            {
                LOG_CHANNEL(channel_name, error) << error_message << exception.what();
            }
            catch (...)
            {
                // 忽略日志记录失败。
            }
        }
        catch (...)
        {
            try
            {
                LOG_CHANNEL(channel_name, fatal) << "unknown error[" << error_message << "]";
            }
            catch (...)
            {
                // 忽略日志记录失败。
            }
        }
    }

    template <typename Func, typename ReturnType>
    [[nodiscard]] ReturnType noexcept_safe_call_and_log(Func f, const std::string_view channel_name, const std::string& error_message, const ReturnType& default_value) noexcept
    {
        try
        {
            return f();
        }
        catch (const std::exception& exception)
        {
            try
            {
                LOG_CHANNEL(channel_name, error) << error_message << exception.what();
            }
            catch (...)
            {
                // 忽略日志记录失败。
            }
        }
        catch (...)
        {
            try
            {
                LOG_CHANNEL(channel_name, fatal) << "unknown error[" << error_message << "]";
            }
            catch (...)
            {
                // 忽略日志记录失败。
            }
        }

        return default_value;
    }

    // 这里error_message必须传值，传引用会导致程序崩溃。
    template <typename Func>
    [[nodiscard]] boost::asio::awaitable<void> noexcept_safe_call_and_log_awaitable(Func f, const std::string_view channel_name, std::string error_message) noexcept
    {
        try
        {
            co_await f();
        }
        catch (const std::exception& exception)
        {
            try
            {
                LOG_CHANNEL(channel_name, error) << error_message << exception.what();
            }
            catch (...)
            {
                // 忽略日志记录失败。
            }
        }
        catch (...)
        {
            try
            {
                LOG_CHANNEL(channel_name, fatal) << "unknown error[" << error_message << "]";
            }
            catch (...)
            {
                // 忽略日志记录失败。
            }
        }
    }

    template <typename Func, typename ReturnType>
    [[nodiscard]] boost::asio::awaitable<ReturnType> noexcept_safe_call_and_log_awaitable(Func f, const std::string_view channel_name, std::string error_message, const ReturnType& default_value) noexcept
    {
        try
        {
            co_return co_await f();
        }
        catch (const std::exception& exception)
        {
            try
            {
                LOG_CHANNEL(channel_name, error) << error_message << exception.what();
            }
            catch (...)
            {
                // 忽略日志记录失败。
            }
        }
        catch (...)
        {
            try
            {
                LOG_CHANNEL(channel_name, fatal) << "unknown error[" << error_message << "]";
            }
            catch (...)
            {
                // 忽略日志记录失败。
            }
        }

        co_return default_value;
    }
}