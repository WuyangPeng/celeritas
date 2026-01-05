#include "test_http_message_handler.h"

std::string celeritas::test_http_message_handler::get_supported_type_name() const
{
    return "test_handler";
}

bool celeritas::test_http_message_handler::handle(const http_handle_parameter_shared_ptr& handle_parameter, const http_message_registry_weak_ptr& message_registry)
{
    was_handled_ = true;
    return true;
}

bool celeritas::test_http_message_handler::is_was_handled() const noexcept
{
    return was_handled_;
}