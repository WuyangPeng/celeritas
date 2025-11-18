#pragma once

#include "database_data_type.h"
#include "database_fwd.h"

#include <any>
#include <memory>
#include <string>
#include <vector>

namespace celeritas
{
    class basis_database
    {
    public:
        using class_type = basis_database;

        using byte_array = std::vector<uint8_t>;
        using object_shared_ptr = std::shared_ptr<basis_database>;
        using object_array = std::vector<object_shared_ptr>;
        using string_array = std::vector<std::string>;
        using int32_array = std::vector<int32_t>;
        using int64_array = std::vector<int64_t>;
        using double_array = std::vector<double>;

    public:
        explicit basis_database(std::string_view fieldName);

        basis_database(std::string_view fieldName, const std::string& value);

        basis_database(std::string_view fieldName, int32_t value);

        basis_database(std::string_view fieldName, int64_t value);

        basis_database(std::string_view fieldName, uint64_t value);

        basis_database(std::string_view fieldName, const byte_array& value);

        basis_database(std::string_view fieldName, bool value);

        basis_database(std::string_view fieldName, const string_array& value);

        basis_database(std::string_view fieldName, const int32_array& value);

        basis_database(std::string_view fieldName, const int64_array& value);

        basis_database(std::string_view fieldName, const double_array& value);

        template <typename T> requires(std::is_integral_v<T>)
        basis_database(std::string_view fieldName, T value);

        template <typename T> requires(std::is_floating_point_v<T>)
        basis_database(std::string_view fieldName, T value);

        [[nodiscard]] std::string_view get_field_name() const noexcept;

        [[nodiscard]] database_data_type get_data_type() const noexcept;

        template <database_data_type Type>
        [[nodiscard]] database_data_Type_traits<Type>::Type get_value() const;

        template <database_data_type Type>
        [[nodiscard]] std::string get_array_string_value() const;

        [[nodiscard]] std::any get_any_value() const;

        [[nodiscard]] std::string get_sql_field_string() const;

        [[nodiscard]] std::string get_string() const;

        [[nodiscard]] std::string get_quotation_mark_string() const;

    private:
        basis_database(std::string_view fieldName, database_data_type dataType, std::any value);

        std::string_view field_name_;
        database_data_type data_type_ = database_data_type::null_type;
        std::any value_;
    };
}