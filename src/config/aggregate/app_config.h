#pragma once

#include "server_config.h"
#include "config/local/database_config.h"
#include "config/local/global_config.h"
#include "config/local/health_check_url_config.h"
#include "config/local/logger_config.h"
#include "config/local/logger_level_config.h"
#include "config/local/service_registry_config.h"

#include <map>

namespace celeritas
{
    class app_config
    {
    public:
        using class_type = app_config;
        using logger_config_container = std::map<std::string, logger_config>;
        using database_config_container = std::map<std::string, database_config>;
        using service_registry_config_container = std::map<std::string, service_registry_config>;
        using const_service_registry_config_container_shared_ptr = std::shared_ptr<const service_registry_config_container>;

        void load_service_registry_config(const std::string& filename);

        void load_server_config(const std::string& filename);

        void load_health_check_url_config(const std::string& filename);

        void load_databases_config(const std::string& filename);

        void load_loggers_config(const std::string& filename);

        void load_global_config(const std::string& filename);

        [[nodiscard]] service_registry_config_container get_service_registry_config() const;

        [[nodiscard]] logger_level_config get_logger_level_config() const;

        [[nodiscard]] logger_config_container get_logger_config() const;

        [[nodiscard]] database_config_container get_database_config() const;

        [[nodiscard]] database_config get_database_config(const std::string& db_name) const;

        [[nodiscard]] server_config get_server_config() const;

        [[nodiscard]] health_check_url_config get_health_check_url_config() const;

        [[nodiscard]] int64_t get_expire_milliseconds(const std::string& db_name) const;

        [[nodiscard]] std::string get_external_host() const;

    private:
        void do_load_databases_config(const std::string& filename);

        void do_load_loggers_config(const std::string& filename);

        void do_load_service_registry_config(const std::string& filename);

        void do_load_global_config(const std::string& filename);

        service_registry_config_container service_registry_;
        server_config server_;
        health_check_url_config health_check_url_;
        logger_level_config logger_level_config_;
        database_config_container database_;
        logger_config_container logger_;
        global_config global_;
    };
}