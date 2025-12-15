#pragma once

#include <boost/json.hpp>

#include <optional>
#include <string>

namespace celeritas
{
    class entity_attribute
    {
    public:
        using class_type = entity_attribute;
        using optional_string = std::optional<std::string>;

        entity_attribute() noexcept = default;

        [[nodiscard]] std::string get_entity_name() const;

        [[nodiscard]] std::string get_data_type() const;

        [[nodiscard]] std::string get_element_type() const;

        [[nodiscard]] optional_string get_index_type() const;

        [[nodiscard]] bool is_bool_type() const;

        [[nodiscard]] bool is_noexcept_type() const;

        [[nodiscard]] bool is_count_type() const;

        [[nodiscard]] bool is_key_type() const;

        [[nodiscard]] bool is_array_type() const;

        void set_entity_name(const std::string& entityName);

        void set_data_type(const std::string& dataType);

        void set_index_type(const std::string& indexType);

    private:
        std::string entity_name_;
        std::string data_type_;
        optional_string index_type_;
    };

    [[nodiscard]] entity_attribute tag_invoke(boost::json::value_to_tag<entity_attribute>, boost::json::value const& value);
}