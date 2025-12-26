#include "mock_resource_loader.h"
#include "service_registry/data/health_check_level_type.h"

celeritas::mock_resource_loader::mock_resource_loader()
    : app_config_{ std::make_shared<app_config>() }
{
}

std::string_view celeritas::mock_resource_loader::get_server_type() const
{
    return "test_server";
}

void celeritas::mock_resource_loader::process_check_tcp_clients_by_duration(io_context_type& io_context)
{
}

void celeritas::mock_resource_loader::process_service_registry_by_duration()
{
}

celeritas::resource_loader_base::app_config_shared_ptr celeritas::mock_resource_loader::get_app_config() const
{
    return app_config_;
}

celeritas::resource_loader_base::health_check_level_awaitable_type celeritas::mock_resource_loader::get_health_check_level()
{
    co_return health_check_level_type::health;
}

bool celeritas::mock_resource_loader::write(const std::string& server_type, const header& header, const protobuf_message& request)
{
    return true;
}

bool celeritas::mock_resource_loader::write(const std::string& server_type, const std::string& instance_id, const header& header, const protobuf_message& request)
{
    return true;
}

bool celeritas::mock_resource_loader::write_to_client(const header& header, const protobuf_message& response)
{
    return true;
}

void celeritas::mock_resource_loader::add_session_route(int64_t user_id, session_route session_route)
{
}

void celeritas::mock_resource_loader::check_client(io_context_type& io_context, const std::string& server_type, const service_info_container& container)
{
}

