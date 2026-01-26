#include "initializer_factory.h"
#include "common/core/celeritas_error.h"
#include "initializer/initializer_constant.h"
#include "initializer/admin_server/admin_application_loader.h"
#include "initializer/admin_server/admin_configuration_loader.h"
#include "initializer/admin_server/admin_resource_loader.h"
#include "initializer/auth_server/auth_application_loader.h"
#include "initializer/auth_server/auth_configuration_loader.h"
#include "initializer/auth_server/auth_resource_loader.h"
#include "initializer/battle_server/battle_application_loader.h"
#include "initializer/battle_server/battle_configuration_loader.h"
#include "initializer/battle_server/battle_resource_loader.h"
#include "initializer/chat_server/chat_application_loader.h"
#include "initializer/chat_server/chat_configuration_loader.h"
#include "initializer/chat_server/chat_resource_loader.h"
#include "initializer/cross_server/cross_application_loader.h"
#include "initializer/cross_server/cross_configuration_loader.h"
#include "initializer/cross_server/cross_resource_loader.h"
#include "initializer/game_server/game_application_loader.h"
#include "initializer/game_server/game_configuration_loader.h"
#include "initializer/game_server/game_resource_loader.h"
#include "initializer/gateway_server/gateway_application_loader.h"
#include "initializer/gateway_server/gateway_configuration_loader.h"
#include "initializer/gateway_server/gateway_resource_loader.h"
#include "initializer/logic_server/logic_application_loader.h"
#include "initializer/logic_server/logic_configuration_loader.h"
#include "initializer/logic_server/logic_resource_loader.h"
#include "initializer/log_server/log_application_loader.h"
#include "initializer/log_server/log_configuration_loader.h"
#include "initializer/log_server/log_resource_loader.h"
#include "initializer/payment_server/payment_application_loader.h"
#include "initializer/payment_server/payment_configuration_loader.h"
#include "initializer/payment_server/payment_resource_loader.h"
#include "initializer/player_server/player_application_loader.h"
#include "initializer/player_server/player_configuration_loader.h"
#include "initializer/player_server/player_resource_loader.h"
#include "initializer/service_registry_server/service_registry_application_loader.h"
#include "initializer/service_registry_server/service_registry_configuration_loader.h"
#include "initializer/service_registry_server/service_registry_resource_loader.h"

celeritas::initializer_factory::configuration_loader_unique_ptr celeritas::initializer_factory::create_configuration_loader(const std::string_view server_type, std::string config_file_path)
{
    if (server_type == service_registry_type)
    {
        return std::make_unique<service_registry_configuration_loader>(config_file_path);
    }

    if (server_type == auth_type)
    {
        return std::make_unique<auth_configuration_loader>(config_file_path);
    }

    if (server_type == gateway_type)
    {
        return std::make_unique<gateway_configuration_loader>(config_file_path);
    }

    if (server_type == player_type)
    {
        return std::make_unique<player_configuration_loader>(config_file_path);
    }

    if (server_type == logic_type)
    {
        return std::make_unique<logic_configuration_loader>(config_file_path);
    }

    if (server_type == game_type)
    {
        return std::make_unique<game_configuration_loader>(config_file_path);
    }

    if (server_type == battle_type)
    {
        return std::make_unique<battle_configuration_loader>(config_file_path);
    }

    if (server_type == payment_type)
    {
        return std::make_unique<payment_configuration_loader>(config_file_path);
    }

    if (server_type == admin_type)
    {
        return std::make_unique<admin_configuration_loader>(config_file_path);
    }

    if (server_type == cross_type)
    {
        return std::make_unique<cross_configuration_loader>(config_file_path);
    }

    if (server_type == chat_type)
    {
        return std::make_unique<chat_configuration_loader>(config_file_path);
    }

    if (server_type == log_type)
    {
        return std::make_unique<log_configuration_loader>(config_file_path);
    }

    throw celeritas_error("unrecognized server type");
}

celeritas::initializer_factory::resource_loader_shared_ptr celeritas::initializer_factory::create_resource_loader(const std::string_view server_type, const app_config_shared_ptr& app_config)
{
    if (server_type == service_registry_type)
    {
        return std::make_shared<service_registry_resource_loader>(server_type, app_config);
    }

    if (server_type == auth_type)
    {
        return std::make_shared<auth_resource_loader>(server_type, app_config);
    }

    if (server_type == gateway_type)
    {
        return std::make_shared<gateway_resource_loader>(server_type, app_config);
    }

    if (server_type == player_type)
    {
        return std::make_shared<player_resource_loader>(server_type, app_config);
    }

    if (server_type == logic_type)
    {
        return std::make_shared<logic_resource_loader>(server_type, app_config);
    }

    if (server_type == game_type)
    {
        return std::make_shared<game_resource_loader>(server_type, app_config);
    }

    if (server_type == battle_type)
    {
        return std::make_shared<battle_resource_loader>(server_type, app_config);
    }

    if (server_type == payment_type)
    {
        return std::make_shared<payment_resource_loader>(server_type, app_config);
    }

    if (server_type == admin_type)
    {
        return std::make_shared<admin_resource_loader>(server_type, app_config);
    }

    if (server_type == cross_type)
    {
        return std::make_shared<cross_resource_loader>(server_type, app_config);
    }

    if (server_type == chat_type)
    {
        return std::make_shared<chat_resource_loader>(server_type, app_config);
    }

    if (server_type == log_type)
    {
        return std::make_shared<log_resource_loader>(server_type, app_config);
    }

    throw celeritas_error("unrecognized server type");
}

celeritas::initializer_factory::application_loader_shared_ptr celeritas::initializer_factory::create_application_loader(const std::string_view server_type, const app_config_shared_ptr& app_config)
{
    if (server_type == service_registry_type)
    {
        return std::make_shared<service_registry_application_loader>(server_type, app_config);
    }

    if (server_type == auth_type)
    {
        return std::make_shared<auth_application_loader>(server_type, app_config);
    }

    if (server_type == gateway_type)
    {
        return std::make_shared<gateway_application_loader>(server_type, app_config);
    }

    if (server_type == player_type)
    {
        return std::make_shared<player_application_loader>(server_type, app_config);
    }

    if (server_type == logic_type)
    {
        return std::make_shared<logic_application_loader>(server_type, app_config);
    }

    if (server_type == game_type)
    {
        return std::make_shared<game_application_loader>(server_type, app_config);
    }

    if (server_type == battle_type)
    {
        return std::make_shared<battle_application_loader>(server_type, app_config);
    }

    if (server_type == payment_type)
    {
        return std::make_shared<payment_application_loader>(server_type, app_config);
    }

    if (server_type == admin_type)
    {
        return std::make_shared<admin_application_loader>(server_type, app_config);
    }

    if (server_type == cross_type)
    {
        return std::make_shared<cross_application_loader>(server_type, app_config);
    }

    if (server_type == chat_type)
    {
        return std::make_shared<chat_application_loader>(server_type, app_config);
    }

    if (server_type == log_type)
    {
        return std::make_shared<log_application_loader>(server_type, app_config);
    }

    throw celeritas_error("unrecognized server type");
}