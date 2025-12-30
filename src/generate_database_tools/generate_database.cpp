#include "generate_database.h"
#include "generate_database_tools_fwd.h"
#include "common/core_utilities/command_line_config.h"
#include "common/logger.h"
#include "detail/generate_database_file.h"

#include <filesystem>

celeritas::generate_database::generate_database(const command_line_config& command_line_config)
    : base_type{ command_line_config },
      database_directory_{ command_line_config.get<std::string>(database_directory_command_line.data()) },
      output_directory_{ command_line_config.get<std::string>(output_directory_command_line.data()) },
      template_directory_{ command_line_config.get<std::string>(template_directory_command_line.data()) },
      database_template_file_{ template_directory_ }
{
    check_directory();
}

void celeritas::generate_database::execute()
{
    try
    {
        generate(database_directory_);
    }
    catch (const std::filesystem::filesystem_error& error)
    {
        LOG_CHANNEL(celeritas::default_channel, error) << "error accessing path:" << error.what();
    }
}

void celeritas::generate_database::check_directory() const
{
    check_directory_exists(database_directory_);
    check_directory_exists(output_directory_);
    check_directory_exists(template_directory_);
}

void celeritas::generate_database::generate(const std::string& directory)
{
    const std::filesystem::path base_directory{ directory };

    for (const auto& entry : std::filesystem::recursive_directory_iterator(directory))
    {
        if (entry.is_regular_file())
        {
            const auto relative_full_path = std::filesystem::relative(entry, base_directory);

            const auto relative_path = relative_full_path.parent_path();

            if (relative_path.filename().string().find("mongo") != std::string::npos)
            {
                generate_file(relative_path.string(), entry.path().string(), true);
            }
            else
            {
                generate_file(relative_path.string(), entry.path().string(), false);
            }
        }
    }
}

void celeritas::generate_database::generate_file(const std::string& relative_path, const std::string& database_file, const bool mongo) const
{
    generate_database_file generate_database_file{ relative_path, database_file, output_directory_, database_template_file_, mongo };
    generate_database_file.execute();
}