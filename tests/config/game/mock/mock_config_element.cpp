#include "mock_config_element.h"

celeritas::mock_config_element::mock_config_element(const int id, std::string value)
    : id_{ id }, value_(std::move(value))
{
}

int celeritas::mock_config_element::get_id() const noexcept
{
    return id_;
}

std::string celeritas::mock_config_element::get_value() const
{
    return value_;
}