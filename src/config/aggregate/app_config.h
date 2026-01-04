#pragma once

#include "server_config.h"
#include "config/config_fwd.h"

#include <map>
#include <memory>

namespace celeritas
{
    class app_config
    {
    public:
        using class_type = app_config;
        using const_service_registry_shared_ptr = std::shared_ptr<const service_registry_config>;
        using service_registry_container = std::map<std::string, const_service_registry_shared_ptr>;
        using const_service_registry_container_shared_ptr = std::shared_ptr<const service_registry_container>;
        using const_server_shared_ptr = std::shared_ptr<const server_config>;
        using const_health_check_url_shared_ptr = std::shared_ptr<const health_check_url_config>;
        using const_database_shared_ptr = std::shared_ptr<const database_config>;
        using database_container = std::map<std::string, const_database_shared_ptr>;
        using const_database_container_shared_ptr = std::shared_ptr<const database_container>;
        using const_logger_shared_ptr = std::shared_ptr<const logger_config>;
        using logger_container = std::map<std::string, const_logger_shared_ptr>;
        using const_logger_container_shared_ptr = std::shared_ptr<const logger_container>;
        using const_global_shared_ptr = std::shared_ptr<const global_config>;
        using const_logger_level_shared_ptr = std::shared_ptr<const logger_level_config>;

        app_config();

        void load_service_registry_config(const std::string& filename);

        void load_server_config(const std::string& filename);

        void load_health_check_url_config(const std::string& filename);

        void load_databases_config(const std::string& filename);

        void load_loggers_config(const std::string& filename);

        void load_global_config(const std::string& filename);

        [[nodiscard]] const_service_registry_container_shared_ptr get_service_registry_config() const;

        [[nodiscard]] const_server_shared_ptr get_server_config() const;

        [[nodiscard]] const_health_check_url_shared_ptr get_health_check_url_config() const;

        [[nodiscard]] const_database_container_shared_ptr get_database_config() const;

        [[nodiscard]] const_database_shared_ptr get_database_config(const std::string& db_name) const;

        [[nodiscard]] const_logger_level_shared_ptr get_logger_level_config() const;

        [[nodiscard]] const_logger_container_shared_ptr get_logger_config() const;

        [[nodiscard]] int64_t get_expire_milliseconds(const std::string& db_name) const;

        [[nodiscard]] std::string get_external_host() const;

    private:
        using service_registry_container_shared_ptr = std::shared_ptr<service_registry_container>;
        using database_container_shared_ptr = std::shared_ptr<database_container>;
        using logger_container_shared_ptr = std::shared_ptr<logger_container>;

        void do_load_service_registry_config(const std::string& filename);

        void do_load_databases_config(const std::string& filename);

        void do_load_loggers_config(const std::string& filename);

        service_registry_container_shared_ptr service_registry_;
        const_server_shared_ptr server_;
        const_health_check_url_shared_ptr health_check_url_;
        database_container_shared_ptr database_;
        const_logger_level_shared_ptr logger_level_config_;
        logger_container_shared_ptr logger_;
        const_global_shared_ptr global_;
    };
}