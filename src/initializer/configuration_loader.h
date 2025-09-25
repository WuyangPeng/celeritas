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

        explicit configuration_loader(std::string_view config_file_path);

        virtual ~configuration_loader() noexcept = default;

        configuration_loader(const configuration_loader& rhs) noexcept = delete;

        configuration_loader& operator=(const configuration_loader& rhs) noexcept = delete;

        configuration_loader(configuration_loader&& rhs) noexcept = delete;

        configuration_loader& operator=(configuration_loader&& rhs) noexcept = delete;

        void initialize();

        [[nodiscard]] app_config get_app_config() const;

    private:
        void initialize_service_registry_config();

        void initialize_server_config();

        void initialize_health_check_url_config();

        void initialize_database_config();

        void initialize_logger_config();

        virtual void service_initialize_config() = 0;

        std::string config_file_path_;
        boost::filesystem::path current_path_;
        boost::filesystem::path config_path_;
        app_config app_config_;
    };
}
