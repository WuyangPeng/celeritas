#pragma once

#include "config/app_config.h"

#include <boost/filesystem.hpp>
#include <string>

namespace celeritas
{
    class configuration_loader
    {
    public:
        using class_type = configuration_loader;
        using app_config_shared_ptr = std::shared_ptr<app_config>;

        explicit configuration_loader(std::string_view config_file_path);

        virtual ~configuration_loader() noexcept = default;

        configuration_loader(const configuration_loader& rhs) noexcept = delete;

        configuration_loader& operator=(const configuration_loader& rhs) noexcept = delete;

        configuration_loader(configuration_loader&& rhs) noexcept = delete;

        configuration_loader& operator=(configuration_loader&& rhs) noexcept = delete;

        void initialize();

        [[nodiscard]] app_config_shared_ptr get_app_config() const;

    private:
        using path_type = boost::filesystem::path;

        void initialize_service_registry_config();

        void initialize_server_config();

        void initialize_health_check_url_config();

        void initialize_database_config();

        void initialize_logger_config();

        virtual void service_initialize_config() = 0;

        std::string config_file_path_;
        path_type current_path_;
        path_type config_path_;
        app_config_shared_ptr app_config_;
    };
}
