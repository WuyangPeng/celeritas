#pragma once

#include "common/logger/logger.h"

namespace celeritas
{
    template <typename Func>
    void noexcept_safe_call_and_log(Func f, const std::string_view log_channel, const std::string& error_message) noexcept
    {
        try
        {
            f();
        }
        catch (const std::exception& exception)
        {
            try
            {
                LOG_CHANNEL(log_channel, error) << error_message << exception.what();
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
                LOG_CHANNEL(log_channel, fatal) << "unknown error: " << error_message;
            }
            catch (...)
            {
                // 忽略日志记录失败。
            }
        }
    }
}