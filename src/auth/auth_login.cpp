#include "auth_login.h"

celeritas::auth_login::auth_login(http_handle_parameter handle_parameter)
    : base_type{ std::move(handle_parameter) }
{
}