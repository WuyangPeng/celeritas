#pragma once

#include "database/basic/database_data_type_traits.h"

#include <bsoncxx/builder/basic/document.hpp>

namespace celeritas
{
    class logs_data
    {
    public:
        using class_type = logs_data;
        using document_type = traits::document_type;

        logs_data();

        explicit logs_data(int64_t expire_time);

        [[nodiscard]] int64_t get_expire_time() const;

        void set_expire_time(int64_t expire_time);

        [[nodiscard]] const std::string& get_string_val() const;

        void set_string_val(std::string string_val);

        [[nodiscard]] const std::vector<std::string>& get_string_array_val() const;

        void set_string_array_val(std::vector<std::string> string_array_val);

        [[nodiscard]] int32_t get_int32_val() const;

        void set_int32_val(int32_t int32_val);

        [[nodiscard]] int32_t get_int32_count_val() const;

        void set_int32_count_val(int32_t int32_count_val);

        [[nodiscard]] const std::vector<int32_t>& get_int32_array_val() const;

        void set_int32_array_val(std::vector<int32_t> int32_array_val);

        [[nodiscard]] int64_t get_int64_count_val() const;

        void set_int64_count_val(int64_t int64_count_val);

        [[nodiscard]] const std::vector<int64_t>& get_int64_array_val() const;

        void set_int64_array_val(std::vector<int64_t> int64_array_val);

        [[nodiscard]] double get_double_val() const;

        void set_double_val(double double_val);

        [[nodiscard]] const std::vector<double>& get_double_array_val() const;

        void set_double_array_val(std::vector<double> double_array_val);

        [[nodiscard]] bool get_bool_val() const;

        void set_bool_val(bool bool_val);

        [[nodiscard]] const document_type& get_document_val() const;

        void set_document_val(document_type document_val);

        [[nodiscard]] const traits::document_array_type& get_document_array_val() const;

        void set_document_array_val(traits::document_array_type document_array_val);

        [[nodiscard]] document_type to_document_type() const;

        [[nodiscard]] static logs_data from_document(const document_type& document);

        static constexpr std::string_view expire_time_description = "expire_time";
        static constexpr std::string_view string_val_description = "string_val";
        static constexpr std::string_view string_array_val_description = "string_array_val";
        static constexpr std::string_view int32_val_description = "int32_val";
        static constexpr std::string_view int32_count_val_description = "int32_count_val";
        static constexpr std::string_view int32_array_val_description = "int32_array_val";
        static constexpr std::string_view int64_count_val_description = "int64_count_val";
        static constexpr std::string_view int64_array_val_description = "int64_array_val";
        static constexpr std::string_view double_val_description = "double_val";
        static constexpr std::string_view double_array_val_description = "double_array_val";
        static constexpr std::string_view bool_val_description = "bool_val";
        static constexpr std::string_view document_val_description = "document_val";
        static constexpr std::string_view document_array_val_description = "document_array_val";

    private:
        int64_t expire_time_;
        std::string string_val_;
        std::vector<std::string> string_array_val_;
        int32_t int32_val_;
        int32_t int32_count_val_;
        std::vector<int32_t> int32_array_val_;
        int64_t int64_count_val_;
        std::vector<int64_t> int64_array_val_;
        double double_val_;
        std::vector<double> double_array_val_;
        bool bool_val_;
        document_type document_val_;
        traits::document_array_type document_array_val_;
    };
}
