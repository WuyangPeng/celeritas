#include "auth_service_base.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

celeritas::auth_service_base::auth_service_base(http_handle_parameter handle_parameter)
    : handle_parameter_{ std::move(handle_parameter) }
{
}

celeritas::auth_service_base::void_awaitable_type celeritas::auth_service_base::write_immediately(const http_response& response) const
{
    co_return co_await handle_parameter_.write_immediately(response.to_json_string());
}

std::string celeritas::auth_service_base::generate_token()
{
    boost::uuids::random_generator generator{};
    const auto uuid = generator();

    return boost::uuids::to_string(uuid);
}

celeritas::auth_service_base::optional_string celeritas::auth_service_base::get_param(const std::string& key) const
{
    return handle_parameter_.get_param(key);
}

celeritas::auth_service_base::app_config_const_shared_ptr celeritas::auth_service_base::get_app_config() const
{
    return handle_parameter_.get_app_config();
}

const celeritas::http_handle_parameter& celeritas::auth_service_base::get_http_handle_parameter() const
{
    return handle_parameter_;
}

void celeritas::auth_service_base::submit_task(task_type task) const
{
    handle_parameter_.submit_task(std::move(task));
}

celeritas::auth_service_base::io_context_type& celeritas::auth_service_base::get_io_context() const
{
    return handle_parameter_.get_io_context();
}
