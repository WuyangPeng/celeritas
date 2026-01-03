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

        [[nodiscard]] std::string get_filename() const;

    private:
        void init(const std::string& content) const;

        void remove_file();

        std::string filename_;
    };
}

