#pragma once

#include <boost/program_options.hpp>

namespace celeritas
{
    class command_line_config
    {
    public:
        using class_type = command_line_config;

        command_line_config(int argc, char** argv, const std::string_view& server_type);

        template <typename T>
        [[nodiscard]] T get(const std::string& key) const;

        [[nodiscard]] bool is_exit_requested() const;

    private:
        boost::program_options::options_description options_desc_;
        boost::program_options::variables_map variables_;
        bool exit_requested_;
    };
}