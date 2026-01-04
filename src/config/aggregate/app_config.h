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
        using const_registry_config_shared_ptr = std::shared_ptr<const service_registry_config>;
        using registry_container = std::map<std::string, const_registry_config_shared_ptr>;
        using const_registry_container_shared_ptr = std::shared_ptr<const registry_container>;
        using const_server_config_shared_ptr = std::shared_ptr<const server_config>;
        using const_health_check_url_config_shared_ptr = std::shared_ptr<const health_check_url_config>;
        using const_database_config_shared_ptr = std::shared_ptr<const database_config>;
        using database_config_container = std::map<std::string, const_database_config_shared_ptr>;
        using const_database_config_container_shared_ptr = std::shared_ptr<const database_config_container>;
        using logger_config_container = std::map<std::string, logger_config>;

        app_config();

        void load_service_registry_config(const std::string& filename);

        void load_server_config(const std::string& filename);

        void load_health_check_url_config(const std::string& filename);

        void load_databases_config(const std::string& filename);

        void load_loggers_config(const std::string& filename);

        void load_global_config(const std::string& filename);

        [[nodiscard]] const_registry_container_shared_ptr get_service_registry_config() const;

        [[nodiscard]] const_server_config_shared_ptr get_server_config() const;

        [[nodiscard]] const_health_check_url_config_shared_ptr get_health_check_url_config() const;

        [[nodiscard]] const_database_config_container_shared_ptr get_database_config() const;

        [[nodiscard]] const_database_config_shared_ptr get_database_config(const std::string& db_name) const;

        [[nodiscard]] logger_level_config get_logger_level_config() const;

        [[nodiscard]] logger_config_container get_logger_config() const;

        [[nodiscard]] int64_t get_expire_milliseconds(const std::string& db_name) const;

        [[nodiscard]] std::string get_external_host() const;

    private:
        using registry_container_shared_ptr = std::shared_ptr<registry_container>;
        using database_config_container_shared_ptr = std::shared_ptr<database_config_container>;

        void do_load_service_registry_config(const std::string& filename);

        void do_load_databases_config(const std::string& filename);

        void do_load_loggers_config(const std::string& filename);

        void do_load_global_config(const std::string& filename);

        registry_container_shared_ptr service_registry_;
        const_server_config_shared_ptr server_;
        const_health_check_url_config_shared_ptr health_check_url_;
        database_config_container_shared_ptr database_;
        logger_level_config logger_level_config_;
        logger_config_container logger_;
        global_config global_;
    };
}