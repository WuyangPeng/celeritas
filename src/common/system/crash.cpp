#include "crash.h"
#include "common/logging/logger.h"

#include <csignal>
#include <stacktrace>

#ifdef WIN32

#include <windows.h>

namespace
{
    LONG WINAPI win32_crash_handler(EXCEPTION_POINTERS* ExceptionInfo)
    {
        const auto exception_code = ExceptionInfo->ExceptionRecord->ExceptionCode;
        auto signal_number = 0;

        switch (exception_code)
        {
            case EXCEPTION_ACCESS_VIOLATION: // 相当于 SIGSEGV (段错误)
            case EXCEPTION_IN_PAGE_ERROR:
            case EXCEPTION_STACK_OVERFLOW:
                signal_number = SIGSEGV;
                break;
            case EXCEPTION_ILLEGAL_INSTRUCTION: // 相当于 SIGILL (非法指令)
                signal_number = SIGILL;
                break;
            case EXCEPTION_FLT_DIVIDE_BY_ZERO: // 相当于 SIGFPE (浮点异常)
            case EXCEPTION_INT_DIVIDE_BY_ZERO:
                signal_number = SIGFPE;
                break;
            case EXCEPTION_BREAKPOINT:
            case EXCEPTION_SINGLE_STEP:
                return EXCEPTION_CONTINUE_SEARCH;
            default:
                signal_number = static_cast<int>(exception_code);
                break;
        }

        celeritas::crash::crash_handler(signal_number);

        return EXCEPTION_EXECUTE_HANDLER;
    }
}

#endif // WIN32

void celeritas::crash::set_signal()
{
    #ifdef WIN32

    if (!IsDebuggerPresent())
    {
        SetUnhandledExceptionFilter(win32_crash_handler);

        signal(SIGABRT, crash_handler);
        signal(SIGFPE, crash_handler);
    }

    #else

    struct sigaction sa{};
    sa.sa_handler = crash_handler;

    sigemptyset(&sa.sa_mask);

    // 注册 SIGSEGV, SIGABRT, SIGFPE
    sigaction(SIGSEGV, &sa, nullptr);
    sigaction(SIGABRT, &sa, nullptr);
    sigaction(SIGFPE, &sa, nullptr);

    #endif // WIN32
}

void celeritas::crash::crash_handler(const int signal_number)
{
    auto trace = std::stacktrace::current();

    // 获取信号名称
    auto signal_name = "UNKNOWN";
    switch (signal_number)
    {
        case SIGSEGV:
            signal_name = "SIGSEGV (Segmentation fault)";
            break;
        case SIGABRT:
            signal_name = "SIGABRT (Abort)";
            break;
        case SIGFPE:
            signal_name = "SIGFPE (Floating point exception)";
            break;
        case SIGILL:
            signal_name = "SIGILL (Illegal instruction)";
            break;
        default:
            break;
    }

    LOG_CHANNEL(initializer_channel, fatal)
        << "CRASH DETECTED!\n"
        << "Signal: "
        << signal_name
        << " ("
        << signal_number
        << ")\n"
        << "Stack trace ("
        << trace.size()
        << " frames):\n"
        << trace;

    for (auto i = 0; i < trace.size(); ++i)
    {
        const auto& entry = trace[i];
        LOG_CHANNEL(initializer_channel, fatal)
            << "#"
            << i
            << " "
            << entry.description()
            << " at "
            << entry.source_file()
            << ":"
            << entry.source_line();
    }

    _exit(signal_number);
}