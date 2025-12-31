#include "mock_celeritas_main.h"
#include "common/core/command_line_config.h"

celeritas::mock_celeritas_main::mock_celeritas_main(const std::string_view& server_type)
    : base_type{ server_type }, create_initializer_called_{}, config_instance_name_{}
{
}

std::string celeritas::mock_celeritas_main::get_mock_server_type() const
{
    return get_server_type();
}

void celeritas::mock_celeritas_main::create_initializer(const command_line_config& command_line_config)
{
    create_initializer_called_ = true;
    config_instance_name_ = command_line_config.get<std::string>("instance_name");
}

bool celeritas::mock_celeritas_main::is_create_initializer_called() const
{
    return create_initializer_called_;
}

std::string celeritas::mock_celeritas_main::get_config_instance_name() const
{
    return config_instance_name_;
}