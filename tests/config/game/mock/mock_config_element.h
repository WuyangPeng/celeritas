#pragma once

#include <string>

namespace celeritas
{
    class mock_config_element
    {
    public:
        using class_type = mock_config_element;

        mock_config_element(int id, std::string value);

        [[nodiscard]] int get_id() const noexcept;

        [[nodiscard]] std::string get_value() const;

    private:
        int id_;
        std::string value_;
    };
}