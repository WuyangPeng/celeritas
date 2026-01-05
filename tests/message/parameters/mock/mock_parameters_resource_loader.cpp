#include "mock_parameters_resource_loader.h"
#include "config/aggregate/app_config.h"
#include "config/aggregate/fixture/config_file_fixture.h"
#include "service_registry/data/health_check_level_type.h"

celeritas::mock_parameters_resource_loader::mock_parameters_resource_loader()
    : base_type{}, app_config_{ create_app_config() }
{
}

std::string_view celeritas::mock_parameters_resource_loader::get_server_type() const
{
    return "mock_server";
}

void celeritas::mock_parameters_resource_loader::process_check_tcp_clients_by_duration(const any_io_executor& any_io_executor)
{
}

void celeritas::mock_parameters_resource_loader::process_service_registry_by_duration()
{
}

celeritas::resource_loader_base::const_app_config_shared_ptr celeritas::mock_parameters_resource_loader::get_app_config() const
{
    return app_config_;
}

celeritas::resource_loader_base::health_check_level_awaitable_type celeritas::mock_parameters_resource_loader::get_health_check_level()
{
    co_return health_check_level_type::health;
}

bool celeritas::mock_parameters_resource_loader::write_to_server(const std::string& server_type, const header& header, const protobuf_message& request)
{
    return true;
}

bool celeritas::mock_parameters_resource_loader::write_to_server(const std::string& server_type, const std::string& instance_id, const header& header_message, const protobuf_message& request)
{
    return true;
}

bool celeritas::mock_parameters_resource_loader::write_to_client(const header& header, const protobuf_message& response)
{
    return true;
}

bool celeritas::mock_parameters_resource_loader::write_to_user(const std::string& server_type, int64_t session_id, const header& header, const protobuf_message& message)
{
    return true;
}

void celeritas::mock_parameters_resource_loader::add_session_route(int64_t user_id, session_route session_route)
{
}

void celeritas::mock_parameters_resource_loader::check_client(const any_io_executor& any_io_executor, const std::string& server_type, const service_info_container& container)
{
}

celeritas::resource_loader_base::const_app_config_shared_ptr celeritas::mock_parameters_resource_loader::create_app_config()
{
    const auto config = std::make_shared<app_config>();
    const auto databases_fixture = config_file_fixture::get_databases();
    config->load_databases_config(databases_fixture.get_filename());

    const auto global_fixture = config_file_fixture::get_global();
    config->load_global_config(global_fixture.get_filename());

    return config;
}

