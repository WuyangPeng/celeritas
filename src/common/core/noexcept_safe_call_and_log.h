#pragma once

#include "common/logging/logger.h"

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
    [[nodiscard]] ReturnType noexcept_safe_call_and_log(Func f, const std::string_view channel_name, const std::string& error_message, const ReturnType& defaultValue) noexcept
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

        return defaultValue;
    }
}