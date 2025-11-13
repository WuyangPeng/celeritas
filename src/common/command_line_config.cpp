#include "command_line_config.h"
#include "boost/url/host_type.hpp"
#include "common/logger.h"

celeritas::command_line_config::command_line_config(const int argc, char** argv, const std::string_view server_type)
    : options_description_{ "Allowed options" }, variables_{}, exit_requested_{ false }
{
    init(argc, argv, server_type);
}

celeritas::command_line_config::command_line_config(const int argc, char** argv, const std::string_view server_type, const options_type& options)
    : options_description_{ "Allowed options" }, variables_{}, exit_requested_{ false }
{
    init(argc, argv, server_type, options);
}

bool celeritas::command_line_config::is_exit_requested() const
{
    return exit_requested_;
}

void celeritas::command_line_config::init(const int argc, char** argv, const std::string_view server_type)
{
    add_options(server_type);
    add_program_options(argc, argv);
    print_help();
}

void celeritas::command_line_config::init(const int argc, char** argv, const std::string_view server_type, const options_type& options)
{
    add_options(server_type);
    add_options(options);
    add_program_options(argc, argv);
    print_help();
}

void celeritas::command_line_config::add_options(const std::string_view server_type)
{
    options_description_.add_options()
        ("help,h", "produce help message")
        (config_file_path_command_line.data(),
         boost::program_options::value<std::string>()->default_value(server_type.data()),
         "The configuration file path for the server.");
}

void celeritas::command_line_config::add_options(const options_type& options)
{
    for (const auto& [name, description] : options)
    {
        options_description_.add_options()
            (name.data(),
             boost::program_options::value<std::string>()->default_value(""),
             description.data());
    }
}

void celeritas::command_line_config::add_program_options(const int argc, char** argv)
{
    try
    {
        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, options_description_), variables_);
        boost::program_options::notify(variables_);
    }
    catch (const boost::program_options::error& error)
    {
        LOG(error) << "Command line parsing error: " << error.what();
        exit_requested_ = true;
    }
}

void celeritas::command_line_config::print_help()
{
    if (variables_.contains("help"))
    {
        LOG(info) << options_description_;
        exit_requested_ = true;
    }
}