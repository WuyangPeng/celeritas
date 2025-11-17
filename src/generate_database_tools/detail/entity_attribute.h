#pragma once

#include <optional>
#include <string>

namespace celeritas
{
    class entity_attribute
    {
    public:
        using class_type = entity_attribute;
        using optional_string = std::optional<std::string>;

        entity_attribute(std::string entity_name, std::string data_type, optional_string index_type);

        [[nodiscard]] std::string get_entity_name() const;

        [[nodiscard]] std::string get_data_type() const;

        [[nodiscard]] optional_string get_index_type() const;

    private:
        std::string entity_name_;
        std::string data_type_;
        optional_string index_type_;
    };
}