#include "payment_service_base.h"
#include "message/http_handle_parameter.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

celeritas::payment_service_base::payment_service_base(http_handle_parameter_shared_ptr handle_parameter)
    : handle_parameter_{ std::move(handle_parameter) }
{
}

celeritas::payment_service_base::void_awaitable_type celeritas::payment_service_base::write_immediately(const http_response& response) const
{
    co_return co_await handle_parameter_->write_immediately(response.to_json_string());
}

std::string celeritas::payment_service_base::generate_token()
{
    boost::uuids::random_generator generator{};
    const auto uuid = generator();

    return boost::uuids::to_string(uuid);
}

celeritas::payment_service_base::optional_string celeritas::payment_service_base::get_param(const std::string& key) const
{
    return handle_parameter_->get_param(key);
}

celeritas::payment_service_base::app_config_const_shared_ptr celeritas::payment_service_base::get_app_config() const
{
    return handle_parameter_->get_app_config();
}

celeritas::payment_service_base::http_handle_parameter_shared_ptr celeritas::payment_service_base::get_http_handle_parameter() const
{
    return handle_parameter_;
}

celeritas::payment_service_base::io_context_type& celeritas::payment_service_base::get_io_context() const
{
    return handle_parameter_->get_io_context();
}
