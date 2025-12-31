#pragma once

#include "common/framework/celeritas_main.h"

namespace celeritas
{
    class mock_celeritas_main final : public celeritas_main
    {
    public:
        using class_type = mock_celeritas_main;
        using base_type = celeritas_main;

        explicit mock_celeritas_main(const std::string_view& server_type);

        [[nodiscard]] std::string get_mock_server_type() const;

        [[nodiscard]] bool is_create_initializer_called() const;

        [[nodiscard]] std::string get_config_instance_name() const;

    private:
        void create_initializer(const command_line_config& command_line_config) override;

        bool create_initializer_called_ = false;
        std::string config_instance_name_;
    };
}