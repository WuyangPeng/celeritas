#pragma once

#include "common/logging/logger.h"

#include <boost/asio/any_io_executor.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <format>

namespace celeritas
{
    template <typename Func, typename... Args>
    void noexcept_safe_call_and_log(Func f,
                                    const std::string_view channel_name,
                                    std::format_string<Args...> format,
                                    Args&&... args) noexcept
    {
        try
        {
            f();
        }
        catch (const std::exception& exception)
        {
            try
            {
                LOG_CHANNEL(channel_name, error) << std::format(format, std::forward<Args>(args)...) << exception.what();
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
                LOG_CHANNEL(channel_name, fatal) << "unknown error[" << std::format(format, std::forward<Args>(args)...) << "]";
            }
            catch (...)
            {
                // 忽略日志记录失败。
            }
        }
    }

    template <typename Func, typename ReturnType, typename... Args>
    [[nodiscard]] ReturnType noexcept_safe_call_and_log(Func f,
                                                        const std::string_view channel_name,
                                                        const ReturnType& default_value,
                                                        std::format_string<Args...> format,
                                                        Args&&... args) noexcept
    {
        try
        {
            return f();
        }
        catch (const std::exception& exception)
        {
            try
            {
                LOG_CHANNEL(channel_name, error) << std::format(format, std::forward<Args>(args)...) << exception.what();
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
                LOG_CHANNEL(channel_name, fatal) << "unknown error[" << std::format(format, std::forward<Args>(args)...) << "]";
            }
            catch (...)
            {
                // 忽略日志记录失败。
            }
        }

        return default_value;
    }

    // 这里参数必须按值捕获，传引用会导致程序崩溃。
    template <typename Func, typename... Args>
    [[nodiscard]] boost::asio::awaitable<void> noexcept_safe_call_and_log_awaitable(Func f,
                                                                                    const std::string_view channel_name,
                                                                                    std::format_string<Args...> format,
                                                                                    Args... args) noexcept
    {
        try
        {
            co_await f();
        }
        catch (const std::exception& exception)
        {
            try
            {
                LOG_CHANNEL(channel_name, error) << std::format(format, std::move(args)...) << exception.what();
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
                LOG_CHANNEL(channel_name, fatal) << "unknown error[" << std::format(format, std::move(args)...) << "]";
            }
            catch (...)
            {
                // 忽略日志记录失败。
            }
        }
    }

    template <typename Func, typename ReturnType, typename... Args>
    [[nodiscard]] boost::asio::awaitable<ReturnType> noexcept_safe_call_and_log_awaitable(Func f,
                                                                                          const std::string_view channel_name,
                                                                                          const ReturnType& default_value,
                                                                                          std::format_string<Args...> format,
                                                                                          Args... args) noexcept
    {
        try
        {
            co_return co_await f();
        }
        catch (const std::exception& exception)
        {
            try
            {
                LOG_CHANNEL(channel_name, error) << std::format(format, std::move(args)...) << exception.what();
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
                LOG_CHANNEL(channel_name, fatal) << "unknown error[" << std::format(format, std::move(args)...) << "]";
            }
            catch (...)
            {
                // 忽略日志记录失败。
            }
        }

        co_return default_value;
    }

    template <typename Func, typename... Args>
    void safe_co_spawn(const boost::asio::any_io_executor& executor,
                       Func&& func,
                       const std::string_view channel_name,
                       std::format_string<Args...> format,
                       Args... args)
    {
        boost::asio::co_spawn(executor,
                              noexcept_safe_call_and_log_awaitable(std::forward<Func>(func),
                                                                   channel_name,
                                                                   format,
                                                                   std::move(args)...),
                              boost::asio::detached);
    }
}
