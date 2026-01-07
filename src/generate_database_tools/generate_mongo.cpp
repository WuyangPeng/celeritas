#include "generate_database_tools_fwd.h"
#include "generate_mongo.h"
#include "common/core/celeritas_error.h"
#include "common/logging/logger.h"
#include "detail/generate_mongo_file.h"
#include "detail/save_mongo_file.h"

#include <filesystem>

celeritas::generate_mongo::generate_mongo(const command_line_config& command_line_config)
    : base_type{ command_line_config },
      database_directory_{ command_line_config.get<std::string>(database_directory_command_line.data()) },
      output_directory_{ command_line_config.get<std::string>(output_directory_command_line.data()) }
{
    check_directory();
}

void celeritas::generate_mongo::execute()
{
    LOG_CHANNEL(celeritas::default_channel, info) << "generate_mongo is executing!";

    try
    {
        generate(database_directory_);
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(celeritas::default_channel, error) << "error accessing path:" << error.what();
    }
}

void celeritas::generate_mongo::check_directory() const
{
    check_directory_exists(database_directory_);
    check_directory_exists(output_directory_);
}

void celeritas::generate_mongo::generate(const std::string& directory)
{
    for (const auto& entry : std::filesystem::recursive_directory_iterator(directory))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".json")
        {
            generate_mongo_file generate_mongo_file{ entry.path().string() };

            const auto content = generate_mongo_file.get_mongo_statement();

            save_mongo_file save_mongo_file{ entry.path().filename().string(), output_directory_, content };
            save_mongo_file.execute();
        }
    }
}
