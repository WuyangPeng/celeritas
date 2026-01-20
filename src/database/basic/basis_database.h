#pragma once

#include "database_data_type.h"
#include "database_data_type_traits.h"
#include "database/database_fwd.h"

#include <memory>
#include <string>
#include <variant>
#include <vector>

namespace celeritas
{
    class basis_database
    {
    public:
        using class_type = basis_database;

        using byte_array = database_data_type_traits<database_data_type::byte_array_type>::type;
        using string_array = database_data_type_traits<database_data_type::string_array_type>::type;
        using int32_array = database_data_type_traits<database_data_type::int32_array_type>::type;
        using int64_array = database_data_type_traits<database_data_type::int64_array_type>::type;
        using double_array = database_data_type_traits<database_data_type::double_array_type>::type;
        using document_type = database_data_type_traits<database_data_type::document_type>::type;
        using document_array = database_data_type_traits<database_data_type::document_array_type>::type;
        using value_variant = std::variant<std::monostate,
                                           std::string,
                                           string_array,
                                           int32_t,
                                           int32_array,
                                           int64_t,
                                           int64_array,
                                           double,
                                           double_array,
                                           bool,
                                           byte_array,
                                           document_type,
                                           document_array>;

        explicit basis_database(std::string_view field_name);

        basis_database(std::string_view field_name, const char* value);

        basis_database(std::string_view field_name, const std::string& value);

        basis_database(std::string_view field_name, int32_t value);

        basis_database(std::string_view field_name, int64_t value);

        basis_database(std::string_view field_name, uint64_t value);

        basis_database(std::string_view field_name, const document_type& value);

        basis_database(std::string_view field_name, const byte_array& value);

        basis_database(std::string_view field_name, bool value);

        basis_database(std::string_view field_name, const string_array& value);

        basis_database(std::string_view field_name, const int32_array& value);

        basis_database(std::string_view field_name, const int64_array& value);

        basis_database(std::string_view field_name, const double_array& value);

        basis_database(std::string_view field_name, const document_array& value);

        template <typename T> requires(std::is_integral_v<T> && sizeof(T) <= sizeof(int32_t))
        basis_database(std::string_view field_name, T value);

        template <typename T> requires(std::is_integral_v<T> && sizeof(T) == sizeof(int64_t))
        basis_database(std::string_view field_name, T value);

        template <typename T> requires(std::is_floating_point_v<T>)
        basis_database(std::string_view field_name, T value);

        [[nodiscard]] std::string_view get_field_name() const noexcept;

        [[nodiscard]] database_data_type get_data_type() const noexcept;

        template <database_data_type Type>
        [[nodiscard]] bool has_value() const;

        template <database_data_type Type>
        [[nodiscard]] const database_data_type_traits<Type>::type& get_value() const;

        template <database_data_type Type>
        [[nodiscard]] std::string get_array_string_value(database_data_type_traits<Type>::param_type value) const;

        [[nodiscard]] const value_variant& get_variant_value() const;

        [[nodiscard]] std::string get_string() const;

        [[nodiscard]] std::string get_quotation_mark_string() const;

        [[nodiscard]] std::string get_sql_value_string() const;

    private:
        basis_database(std::string_view field_name, database_data_type dataType, value_variant value);

        [[nodiscard]] static std::string get_double_string(database_data_type_traits<database_data_type::double_type>::param_type value);

        [[nodiscard]] static std::string get_document_string(database_data_type_traits<database_data_type::document_type>::param_type document);

        [[nodiscard]] static std::string get_document_array_string(database_data_type_traits<database_data_type::document_array_type>::param_type document_array);

        template <database_data_type Type>
        static void append_value(std::ostringstream& os, database_data_type_traits<Type>::element_param_type value, bool is_last);

        static void append_value(std::ostringstream& os, const basis_database& value, bool is_last);

        static void append_value(std::ostringstream& os, const document_type& document, bool is_last);

        static void append_document_value(std::ostringstream& os, const document_type& document);

        std::string field_name_;
        database_data_type data_type_ = database_data_type::null_type;
        value_variant value_;
    };

    [[nodiscard]] bool operator==(const basis_database& lhs, const basis_database& rhs);

    [[nodiscard]] bool operator!=(const basis_database& lhs, const basis_database& rhs);
}
