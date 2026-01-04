#include "game_config.h"
#include "game_tables.h"
#include "common/logging/logger.h"
#include "config/config_constant.h"

#include <filesystem>

celeritas::game_config& celeritas::game_config::get_instance()
{
    static game_config instance{};

    return instance;
}

celeritas::game_config::const_game_tables_shared_ptr celeritas::game_config::get_game_tables()
{
    std::shared_lock lock{ shared_mutex_ };

    return game_tables_;
}

void celeritas::game_config::set_game_tables(const const_game_tables_shared_ptr& game_tables)
{
    std::lock_guard lock{ shared_mutex_ };

    game_tables_ = game_tables;
}

void celeritas::game_config::load_tables()
{
    const auto current_path = std::filesystem::current_path();
    const auto bin_directory = current_path / config_path / bin_path;

    auto loader = [&](luban::ByteBuf& buffer, const std::string& bin_file_name) -> bool {
        const auto full_path = bin_directory / (bin_file_name + ".bytes");

        std::ifstream stream{ full_path, std::ios::binary | std::ios::ate };
        if (!stream.is_open())
        {
            throw std::runtime_error("Cannot open config file: " + full_path.string());
        }

        const auto size = stream.tellg();
        stream.seekg(0, std::ios::beg);

        std::vector<char> data(size);
        if (stream.read(data.data(), size))
        {
            buffer.appendBuffer(data.data(), size);
            return true;
        }

        return false;
    };

    try
    {
        const auto tables = std::make_shared<config::tables>();
        tables->load(loader);

        const auto game = std::make_shared<game_tables>(tables);

        get_instance().set_game_tables(game);
    }
    catch (std::exception& e)
    {
        LOG_CHANNEL(config_channel, error) << "load game config error:" << e.what();
    }
}

celeritas::game_config::game_config()
    : game_tables_{ std::make_shared<game_tables>(std::make_shared<config::tables>()) }
{
}