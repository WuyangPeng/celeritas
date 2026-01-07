#pragma once

#include <boost/json.hpp>

#include <string>
#include <vector>

namespace celeritas
{
    class generate_mongo_file
    {
    public:
        using class_type = generate_mongo_file;

        explicit generate_mongo_file(std::string database_file);

        [[nodiscard]] std::string get_mongo_statement() const;

    private:
        using json_value_type = boost::json::value;
        using index_type = std::vector<std::string>;

        [[nodiscard]] json_value_type get_json_value() const;

        [[nodiscard]] static std::string get_mongo_statement(const json_value_type& table_value);

        std::string database_file_;
    };
}
