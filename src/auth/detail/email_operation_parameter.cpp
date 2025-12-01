#include "auth_internal_fwd.h"
#include "email_operation_parameter.h"
#include "auth/auth_service_base.h"

#include <boost/lexical_cast.hpp>

celeritas::email_operation_parameter::email_operation_parameter(const http_handle_parameter& http_handle_parameter)
    : base_type{ http_handle_parameter }, code_{}
{
}

int celeritas::email_operation_parameter::get_code() const
{
    return code_;
}

celeritas::auth_parameter::optional_http_response celeritas::email_operation_parameter::get_http_parameter()
{
    if (const auto http_response = base_type::get_http_parameter())
    {
        return http_response;
    }

    const auto optional_code = get_param(code_describe.data());
    if (!optional_code)
    {
        return http_response{ game_error_type::invalid_parameter, "code is required" };
    }

    code_ = boost::lexical_cast<int>(*optional_code);

    return std::nullopt;
}
