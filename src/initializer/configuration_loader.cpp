#include "configuration_loader.h"
#include "server/server_fwd.h"

using namespace std::literals;

celeritas::configuration_loader::configuration_loader(const std::string_view config_file_path)
    : config_file_path_{ config_file_path },
      current_path_{ boost::filesystem::current_path() },
      config_path_{ current_path_ / config_path },
      app_config_{ std::make_shared<app_config>() }
{
}

void celeritas::configuration_loader::initialize()
{
    initialize_service_registry_config();
    initialize_server_config();
    initialize_health_check_url_config();
    initialize_database_config();
    initialize_logger_config();
    service_initialize_config();
}

celeritas::configuration_loader::app_config_shared_ptr celeritas::configuration_loader::get_app_config() const
{
    return app_config_;
}

void celeritas::configuration_loader::initialize_service_registry_config()
{
    const auto filename = config_path_ / service_registry_xml;

    app_config_->load_service_registry_config(filename.string());
}

void celeritas::configuration_loader::initialize_server_config()
{
    const auto filename = config_path_ / config_file_path_ / server_xml;

    app_config_->load_server_config(filename.string());
}

void celeritas::configuration_loader::initialize_health_check_url_config()
{
    const auto filename = config_path_ / config_file_path_ / health_check_url_xml;

    app_config_->load_health_check_url_config(filename.string());
}

void celeritas::configuration_loader::initialize_database_config()
{
    const auto main_filename = config_path_ / databases_xml;

    app_config_->load_databases_config(main_filename.string());

    const auto server_filename = config_path_ / config_file_path_ / databases_xml;

    app_config_->load_databases_config(server_filename.string());
}

void celeritas::configuration_loader::initialize_logger_config()
{
    const auto main_filename = config_path_ / loggers_xml;

    app_config_->load_loggers_config(main_filename.string());

    const auto server_filename = config_path_ / config_file_path_ / loggers_xml;

    app_config_->load_loggers_config(server_filename.string());
}
