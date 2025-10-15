#include "command_line_config.h"
#include "logger.h"

celeritas::command_line_config::command_line_config(const int argc, char** argv, const std::string_view& server_type)
    : options_desc_{ "Allowed options" }, variables_{}, exit_requested_{ false }
{
    options_desc_.add_options()
        ("help,h", "produce help message")
        ("config_file_path",
         boost::program_options::value<std::string>()->default_value(server_type.data()),
         "The configuration file path for the server.");

    try
    {
        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, options_desc_), variables_);
        boost::program_options::notify(variables_);
    }
    catch (const boost::program_options::error& error)
    {
        LOG_CHANNEL(common_channel, error) << "Command line parsing error: " << error.what();
        exit_requested_ = true;
        return;
    }

    if (variables_.contains("help"))
    {
        LOG_CHANNEL(common_channel, info) << options_desc_;
        exit_requested_ = true;
    }
}

bool celeritas::command_line_config::is_exit_requested() const
{
    return exit_requested_;
}