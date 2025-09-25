#pragma once

#include "database_config.h"
#include "health_check_url_config.h"
#include "logger_config.h"
#include "logger_level_config.h"
#include "server_config.h"
#include "service_registry_config.h"

#include <map>
#include <vector>

namespace celeritas
{
    class app_config
    {
    public:
        using class_type = app_config;

        void load_service_registry_config(const std::string& filename);

        void load_server_config(const std::string& filename);

        void load_health_check_url_config(const std::string& filename);

        void load_databases_config(const std::string& filename);

        void load_loggers_config(const std::string& filename);

    private:
        using database_config_container = std::map<std::string, database_config>;
        using logger_config_container = std::map<std::string, logger_config>;

        void do_load_databases_config(const std::string& filename);

        void do_load_loggers_config(const std::string& filename);

        service_registry_config service_registry_;
        server_config server_;
        health_check_url_config health_check_url_;
        logger_level_config logger_level_config_;
        database_config_container database_;
        logger_config_container logger_;
    };
}