#pragma once

#include <boost/json.hpp>

#include <string>

namespace celeritas
{
    class generate_mysql_file
    {
    public:
        using class_type = generate_mysql_file;

        explicit generate_mysql_file(std::string database_file);

        [[nodiscard]] std::string get_mysql_statement() const;

    private:
        using json_value_type = boost::json::value;
        using index_type = std::vector<std::string>;

        [[nodiscard]] static std::string get_mysql_data_type(const std::string& data_type);

        [[nodiscard]] json_value_type get_json_value() const;

        [[nodiscard]] std::string get_mysql_statement(const json_value_type& table_value) const;

        std::string database_file_;
    };
}