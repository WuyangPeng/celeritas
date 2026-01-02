#pragma once

#include "celeritas_error.h"

#include <boost/program_options.hpp>

namespace celeritas
{
    class command_line_config
    {
    public:
        using class_type = command_line_config;
        using options_type = std::map<std::string, std::string>;

        command_line_config(int argc, char** argv, std::string_view server_type);

        command_line_config(int argc, char** argv, std::string_view server_type, const options_type& options);

        template <typename T>
        [[nodiscard]] T get(const std::string& key) const
        {
            if (variables_.contains(key))
            {
                return variables_[key].as<T>();
            }

            throw celeritas_error{ "attempted to access unregistered or invalid key: {}", key };
        }

        [[nodiscard]] bool is_exit_requested() const noexcept;

    private:
        using options_description_type = boost::program_options::options_description;
        using variables_map_type = boost::program_options::variables_map;

        void init(int argc, char** argv, std::string_view server_type);

        void init(int argc, char** argv, std::string_view server_type, const options_type& options);

        void add_options(std::string_view server_type);

        void add_options(const options_type& options);

        void add_program_options(int argc, char** argv);

        void do_add_program_options(int argc, char** argv);

        void print_help();

        options_description_type options_description_;
        variables_map_type variables_;
        bool exit_requested_;
    };
}