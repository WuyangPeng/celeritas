#pragma once

#include <string>

namespace celeritas
{
    class mongo_parameter
    {
    public:
        using class_type = mongo_parameter;

        mongo_parameter(std::string uri, std::string db_name);

        [[nodiscard]] std::string get_uri() const;

        [[nodiscard]] std::string get_db_name() const;

    private:
        std::string uri_;
        std::string db_name_;
    };
}