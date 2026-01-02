#include "capture_clog.h"
#include "common/core/noexcept_safe_call_and_log.h"

#include <iostream>

celeritas::capture_clog::capture_clog()
{
    old_buffer = std::clog.rdbuf(buffer.rdbuf());
}

celeritas::capture_clog::~capture_clog() noexcept
{
    noexcept_safe_call_and_log([this] {
                                   std::clog.rdbuf(old_buffer);
                               },
                               common_channel,
                               "timer elapsed error: ");
}

std::string celeritas::capture_clog::str() const
{
    return buffer.str();
}