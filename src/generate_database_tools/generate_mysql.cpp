#include "generate_database_tools_fwd.h"
#include "generate_mysql.h"
#include "common/celeritas_error.h"
#include "common/logger.h"
#include "detail/generate_mysql_file.h"
#include "detail/save_mysql_file.h"

#include <filesystem>

celeritas::generate_mysql::generate_mysql(const command_line_config& command_line_config)
    : base_type{ command_line_config },
      database_directory_{ command_line_config.get<std::string>(database_directory_command_line.data()) },
      output_directory_{ command_line_config.get<std::string>(output_directory_command_line.data()) }
{
    check_directory();
}

void celeritas::generate_mysql::execute()
{
    LOG_CHANNEL(celeritas::default_channel, info) << "generate_mysql is executing!";

    try
    {
        generate(database_directory_);
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(celeritas::default_channel, error) << "error accessing path:" << error.what();
    }
}

void celeritas::generate_mysql::check_directory() const
{
    check_directory_exists(database_directory_);
    check_directory_exists(output_directory_);
}

void celeritas::generate_mysql::generate(const std::string& directory)
{
    for (const auto& entry : std::filesystem::recursive_directory_iterator(directory))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".json")
        {
            generate_mysql_file generate_mysql_file{ entry.path().string() };

            const auto content = generate_mysql_file.get_mysql_statement();

            save_mysql_file save_mysql_file{ entry.path().filename().string(), output_directory_, content };
            save_mysql_file.execute();
        }
    }
}

