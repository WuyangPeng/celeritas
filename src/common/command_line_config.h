#pragma once

#include <boost/program_options.hpp>

namespace celeritas
{
    class command_line_config
    {
    public:
        using class_type = command_line_config;

        command_line_config(int argc, char** argv, std::string_view server_type);

        template <typename T>
        [[nodiscard]] T get(const std::string& key) const;

        [[nodiscard]] bool is_exit_requested() const;

    private:
        using options_description_type = boost::program_options::options_description;
        using variables_map_type = boost::program_options::variables_map;

        void init(int argc, char** argv, std::string_view server_type);

        void add_options(std::string_view server_type);

        void add_program_options(int argc, char** argv);

        void print_help();

        options_description_type options_description_;
        variables_map_type variables_;
        bool exit_requested_;
    };
}