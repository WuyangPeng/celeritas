#pragma once

#include <string>
#include <vector>

namespace celeritas
{
    class scan_result
    {
    public:
        using class_type = scan_result;
        using array_type = std::vector<std::string>;

        scan_result(std::string cursor, array_type keys);

        [[nodiscard]] std::string get_cursor() const;

        [[nodiscard]] array_type get_keys() const;

    private:
        std::string cursor_;
        array_type keys_;
    };
}