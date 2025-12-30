#include "generate_handler.h"
#include "generate_handler_tools_fwd.h"
#include "common/core/celeritas_error.h"
#include "common/core/command_line_config.h"
#include "common/logger/logger.h"
#include "detail/generate_handler_file.h"

#include <filesystem>

celeritas::generate_handler::generate_handler(const command_line_config& command_line_config)
    : base_type{ command_line_config },
      proto_directory_{ command_line_config.get<std::string>(proto_directory_command_line.data()) },
      output_directory_{ command_line_config.get<std::string>(output_directory_command_line.data()) },
      template_directory_{ command_line_config.get<std::string>(template_directory_command_line.data()) },
      handler_template_file_{ template_directory_ }
{
    check_directory();
}

void celeritas::generate_handler::execute()
{
    try
    {
        generate(proto_directory_);
    }
    catch (const std::filesystem::filesystem_error& error)
    {
        LOG_CHANNEL(celeritas::default_channel, error) << "error accessing path:" << error.what();
    }
}

void celeritas::generate_handler::check_directory() const
{
    check_directory_exists(proto_directory_);
    check_directory_exists(output_directory_);
    check_directory_exists(template_directory_);
}

void celeritas::generate_handler::generate(const std::string& directory)
{
    for (const auto& entry : std::filesystem::recursive_directory_iterator(directory))
    {
        if (entry.is_regular_file())
        {
            if (const auto& file_path = entry.path();
                file_path.extension() == proto_extension)
            {
                generate_file(file_path.string());
            }
        }
    }
}

void celeritas::generate_handler::generate_file(const std::string& proto_file) const
{
    generate_handler_file generate_handler_file{ proto_file, proto_directory_, output_directory_, handler_template_file_ };

    generate_handler_file.generate_file();
}
