#include "generate_handler_header.h"
#include "generate_handler_tools_fwd.h"
#include "common/logger/logger.h"
#include "detail/generate_handler_header_file.h"

#include <filesystem>

celeritas::generate_handler_header::generate_handler_header(const command_line_config& command_line_config)
    : base_type{ command_line_config },
      handler_directory_{ command_line_config.get<std::string>(handler_directory_command_line.data()) },
      output_directory_{ command_line_config.get<std::string>(output_directory_command_line.data()) },
      template_directory_{ command_line_config.get<std::string>(template_directory_command_line.data()) },
      handler_file_{}
{
    check_directory();
}

void celeritas::generate_handler_header::execute()
{
    try
    {
        set_handler_file(handler_directory_);
        generate();
    }
    catch (const std::filesystem::filesystem_error& error)
    {
        LOG_CHANNEL(celeritas::default_channel, error) << "error accessing path:" << error.what();
    }
}

void celeritas::generate_handler_header::check_directory()
{
    check_directory_exists(handler_directory_);
    check_directory_exists(output_directory_);
    check_directory_exists(template_directory_);
}

void celeritas::generate_handler_header::set_handler_file(const std::string& directory)
{
    for (const auto& entry : std::filesystem::recursive_directory_iterator(directory))
    {
        if (entry.is_regular_file())
        {
            if (const auto& file_path = entry.path();
                file_path.extension() == header_extension)
            {
                if (file_path.string().find("_handler") != std::string::npos)
                {
                    handler_file_.emplace_back(file_path.string());
                }
            }
        }
    }
}

void celeritas::generate_handler_header::generate()
{
    generate_handler_header_file generate_handler_header_file{ handler_directory_, output_directory_, template_directory_, handler_file_ };
    generate_handler_header_file.execute();
}