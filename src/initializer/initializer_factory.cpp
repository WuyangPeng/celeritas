#include "initializer_factory.h"
#include "admin_server/admin_application_loader.h"
#include "admin_server/admin_configuration_loader.h"
#include "admin_server/admin_resource_loader.h"
#include "auth_server/auth_application_loader.h"
#include "auth_server/auth_configuration_loader.h"
#include "auth_server/auth_resource_loader.h"
#include "battle_server/battle_application_loader.h"
#include "battle_server/battle_configuration_loader.h"
#include "battle_server/battle_resource_loader.h"
#include "chat_server/chat_application_loader.h"
#include "chat_server/chat_configuration_loader.h"
#include "chat_server/chat_resource_loader.h"
#include "common/celeritas_error.h"
#include "cross_server/cross_application_loader.h"
#include "cross_server/cross_configuration_loader.h"
#include "cross_server/cross_resource_loader.h"
#include "game_server/game_application_loader.h"
#include "game_server/game_configuration_loader.h"
#include "game_server/game_resource_loader.h"
#include "gateway_server/gateway_application_loader.h"
#include "gateway_server/gateway_configuration_loader.h"
#include "gateway_server/gateway_resource_loader.h"
#include "logic_server/logic_application_loader.h"
#include "logic_server/logic_configuration_loader.h"
#include "logic_server/logic_resource_loader.h"
#include "payment_server/payment_application_loader.h"
#include "payment_server/payment_configuration_loader.h"
#include "payment_server/payment_resource_loader.h"
#include "player_server/player_application_loader.h"
#include "player_server/player_configuration_loader.h"
#include "player_server/player_resource_loader.h"
#include "server/server_fwd.h"
#include "service_registry_server/service_registry_application_loader.h"
#include "service_registry_server/service_registry_configuration_loader.h"
#include "service_registry_server/service_registry_resource_loader.h"

celeritas::initializer_factory::configuration_loader_unique_ptr celeritas::initializer_factory::create_configuration_loader(const std::string_view& server_type, std::string config_file_path)
{
    if (server_type == service_registry_type)
    {
        return std::make_unique<service_registry_configuration_loader>(config_file_path);
    }
    else if (server_type == auth_type)
    {
        return std::make_unique<auth_configuration_loader>(config_file_path);
    }
    else if (server_type == gateway_type)
    {
        return std::make_unique<gateway_configuration_loader>(config_file_path);
    }
    else if (server_type == player_type)
    {
        return std::make_unique<player_configuration_loader>(config_file_path);
    }
    else if (server_type == logic_type)
    {
        return std::make_unique<logic_configuration_loader>(config_file_path);
    }
    else if (server_type == game_type)
    {
        return std::make_unique<game_configuration_loader>(config_file_path);
    }
    else if (server_type == battle_type)
    {
        return std::make_unique<battle_configuration_loader>(config_file_path);
    }
    else if (server_type == payment_type)
    {
        return std::make_unique<payment_configuration_loader>(config_file_path);
    }
    else if (server_type == admin_type)
    {
        return std::make_unique<admin_configuration_loader>(config_file_path);
    }
    else if (server_type == cross_type)
    {
        return std::make_unique<cross_configuration_loader>(config_file_path);
    }
    else if (server_type == chat_type)
    {
        return std::make_unique<chat_configuration_loader>(config_file_path);
    }

    throw celeritas_error("unrecognized server type");
}

celeritas::initializer_factory::resource_loader_shared_ptr celeritas::initializer_factory::create_resource_loader(const std::string_view& server_type, const app_config_shared_ptr& app_config)
{
    if (server_type == service_registry_type)
    {
        return std::make_shared<service_registry_resource_loader>(app_config);
    }
    else if (server_type == auth_type)
    {
        return std::make_unique<auth_resource_loader>(app_config);
    }
    else if (server_type == gateway_type)
    {
        return std::make_unique<gateway_resource_loader>(app_config);
    }
    else if (server_type == player_type)
    {
        return std::make_unique<player_resource_loader>(app_config);
    }
    else if (server_type == logic_type)
    {
        return std::make_unique<logic_resource_loader>(app_config);
    }
    else if (server_type == game_type)
    {
        return std::make_unique<game_resource_loader>(app_config);
    }
    else if (server_type == battle_type)
    {
        return std::make_unique<battle_resource_loader>(app_config);
    }
    else if (server_type == payment_type)
    {
        return std::make_unique<payment_resource_loader>(app_config);
    }
    else if (server_type == admin_type)
    {
        return std::make_unique<admin_resource_loader>(app_config);
    }
    else if (server_type == cross_type)
    {
        return std::make_unique<cross_resource_loader>(app_config);
    }
    else if (server_type == chat_type)
    {
        return std::make_unique<chat_resource_loader>(app_config);
    }

    throw celeritas_error("unrecognized server type");
}

celeritas::initializer_factory::application_loader_unique_ptr celeritas::initializer_factory::create_application_loader(const std::string_view& server_type, const app_config_shared_ptr& app_config)
{
    if (server_type == service_registry_type)
    {
        return std::make_unique<service_registry_application_loader>(app_config);
    }
    else if (server_type == auth_type)
    {
        return std::make_unique<auth_application_loader>(app_config);
    }
    else if (server_type == gateway_type)
    {
        return std::make_unique<gateway_application_loader>(app_config);
    }
    else if (server_type == player_type)
    {
        return std::make_unique<player_application_loader>(app_config);
    }
    else if (server_type == logic_type)
    {
        return std::make_unique<logic_application_loader>(app_config);
    }
    else if (server_type == game_type)
    {
        return std::make_unique<game_application_loader>(app_config);
    }
    else if (server_type == battle_type)
    {
        return std::make_unique<battle_application_loader>(app_config);
    }
    else if (server_type == payment_type)
    {
        return std::make_unique<payment_application_loader>(app_config);
    }
    else if (server_type == admin_type)
    {
        return std::make_unique<admin_application_loader>(app_config);
    }
    else if (server_type == cross_type)
    {
        return std::make_unique<cross_application_loader>(app_config);
    }
    else if (server_type == chat_type)
    {
        return std::make_unique<chat_application_loader>(app_config);
    }

    throw celeritas_error("unrecognized server type");
}