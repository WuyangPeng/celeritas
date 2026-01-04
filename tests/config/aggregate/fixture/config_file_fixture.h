#pragma once

#include <string>

namespace celeritas
{
    class config_file_fixture
    {
    public:
        using class_type = config_file_fixture;

        config_file_fixture(std::string name, const std::string& content);

        ~config_file_fixture() noexcept;

        [[nodiscard]] static config_file_fixture get_global();

        [[nodiscard]] static config_file_fixture get_health_check_url();

        [[nodiscard]] static config_file_fixture get_databases();

        [[nodiscard]] static config_file_fixture get_loggers();

        [[nodiscard]] static config_file_fixture get_service_registry();

        [[nodiscard]] static config_file_fixture get_server();

        [[nodiscard]] static config_file_fixture get_server_invalid_datacenter_id();

        [[nodiscard]] static config_file_fixture get_server_invalid_worker_id();

        [[nodiscard]] static config_file_fixture get_server_invalid_network_type();

        [[nodiscard]] static config_file_fixture get_databases_redis_invalid_expire();

        [[nodiscard]] std::string get_filename() const;

    private:
        void init(const std::string& content) const;

        void remove_file() const;

        std::string filename_;
    };
}
