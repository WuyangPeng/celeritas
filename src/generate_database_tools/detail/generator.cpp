#include "generator.h"
#include "common/logger/logger.h"

#include <fstream>

bool celeritas::generator::is_content_same(const std::string& file_name, const std::string& content)
{
    if (std::ifstream stream{ file_name, std::ios::binary };
        stream.good())
    {
        std::string existing_content{};
        existing_content.assign((std::istreambuf_iterator{ stream }), (std::istreambuf_iterator<char>()));

        if (existing_content == content)
        {
            LOG_CHANNEL(default_channel, debug) << "File " << file_name << " content unchanged, skipping write.";
            return true;
        }
    }

    return false;
}

void celeritas::generator::save_database(const std::string& file_name, const std::string& content)
{
    std::ofstream os{ file_name, std::ios::binary };

    os << content;

    LOG_CHANNEL(celeritas::default_channel, info) << "generate file : " << file_name;
}