#pragma once

#include "database/basic/basis_database.h"

namespace celeritas
{
    class logs_data
    {
    public:
        using class_type = logs_data;
        using document_type = traits::document_type;
        using string_array_type = traits::string_array_type;
        using int32_array_type = traits::int32_array_type;
        using int64_array_type = traits::int64_array_type;
        using double_array_type = traits::double_array_type;
        using document_array_type = traits::document_array_type;

        logs_data();

        [[nodiscard]] int64_t get_int64_value() const;

        void set_int64_value(int64_t int64_value);

        [[nodiscard]] const std::string& get_string_value() const;

        void set_string_value(std::string string_value);

        [[nodiscard]] const string_array_type& get_string_array_value() const;

        void set_string_array_value(string_array_type string_array_value);

        [[nodiscard]] int32_t get_int32_value() const;

        void set_int32_value(int32_t int32_value);

        [[nodiscard]] int32_t get_int32_count_value() const;

        void set_int32_count_value(int32_t int32_count_value);

        [[nodiscard]] const int32_array_type& get_int32_array_value() const;

        void set_int32_array_value(int32_array_type int32_array_value);

        [[nodiscard]] int64_t get_int64_count_value() const;

        void set_int64_count_value(int64_t int64_count_value);

        [[nodiscard]] const int64_array_type& get_int64_array_value() const;

        void set_int64_array_value(int64_array_type int64_array_value);

        [[nodiscard]] double get_double_value() const;

        void set_double_value(double double_value);

        [[nodiscard]] const double_array_type& get_double_array_value() const;

        void set_double_array_value(double_array_type double_array_value);

        [[nodiscard]] bool get_bool_value() const;

        void set_bool_value(bool bool_value);

        [[nodiscard]] const document_type& get_document_value() const;

        void set_document_value(document_type document_value);

        [[nodiscard]] const document_array_type& get_document_array_value() const;

        void set_document_array_value(document_array_type document_array_value);

        [[nodiscard]] document_type to_document_type() const;

        [[nodiscard]] static logs_data from_document(const document_type& document);

        static constexpr std::string_view int64_value_description = "int64_value";
        static constexpr std::string_view string_value_description = "string_value";
        static constexpr std::string_view string_array_value_description = "string_array_value";
        static constexpr std::string_view int32_value_description = "int32_value";
        static constexpr std::string_view int32_count_value_description = "int32_count_value";
        static constexpr std::string_view int32_array_value_description = "int32_array_value";
        static constexpr std::string_view int64_count_value_description = "int64_count_value";
        static constexpr std::string_view int64_array_value_description = "int64_array_value";
        static constexpr std::string_view double_value_description = "double_value";
        static constexpr std::string_view double_array_value_description = "double_array_value";
        static constexpr std::string_view bool_value_description = "bool_value";
        static constexpr std::string_view document_value_description = "document_value";
        static constexpr std::string_view document_array_value_description = "document_array_value";

    private:
        int64_t int64_value_;
        std::string string_value_;
        string_array_type string_array_value_;
        int32_t int32_value_;
        int32_t int32_count_value_;
        int32_array_type int32_array_value_;
        int64_t int64_count_value_;
        int64_array_type int64_array_value_;
        double double_value_;
        double_array_type double_array_value_;
        bool bool_value_;
        document_type document_value_;
        document_array_type document_array_value_;
    };
}
