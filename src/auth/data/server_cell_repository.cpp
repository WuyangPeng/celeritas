#include "server_cell_repository.h"
#include "common/logger/logger.h"
#include "common/core/time_helper.h"
#include "database/database_pool_manager.h"

#include <ranges>

celeritas::server_cell_repository& celeritas::server_cell_repository::get_instance()
{
    static server_cell_repository instance{};

    return instance;
}

void celeritas::server_cell_repository::reload_from_db(io_context_type& io_context, int64_t cell_id)
{
    if (cell_id == 0)
    {
        load_from_db(io_context);
    }

    boost::asio::co_spawn(io_context, [cell_id, this] {
                              return this->load_from_db(cell_id);
                          },
                          boost::asio::detached);
}

void celeritas::server_cell_repository::load_from_db(io_context_type& io_context)
{
    boost::asio::co_spawn(io_context, [this] {
                              return this->load_from_db();
                          },
                          boost::asio::detached);
}

celeritas::server_cell_repository::optional_server_cell_type celeritas::server_cell_repository::get_server_cell(const std::string& game_server_id)
{
    if (const auto iter = game_server_.find(game_server_id);
        iter != game_server_.cend())
    {
        return iter->second;
    }

    return std::nullopt;
}

celeritas::server_cell_repository::optional_server_cell_type celeritas::server_cell_repository::get_last_server_cell(const int64_t app_id)
{
    if (const auto iter = app_id_server_.find(app_id);
        iter != app_id_server_.cend() && !iter->second.empty())
    {
        const auto current_milliseconds = time_helper::get_current_milliseconds();
        for (auto server_cell = iter->second.cbegin(); server_cell != iter->second.cend(); ++server_cell)
        {
            if (current_milliseconds >= server_cell->get_launch_time())
            {
                return *server_cell;
            }
        }
    }

    return std::nullopt;
}

celeritas::server_cell_repository::server_cell_container_type celeritas::server_cell_repository::get_server_cell_by_app_id(const int64_t app_id, const optional_string& zone)
{
    if (const auto iter = app_id_server_.find(app_id);
        iter != app_id_server_.cend())
    {
        const auto current_milliseconds = time_helper::get_current_milliseconds();

        server_cell_container_type container{};
        for (const auto& server_cell : iter->second)
        {
            if ((!zone || server_cell.get_zone() == zone) && current_milliseconds >= server_cell.get_launch_time())
            {
                container.emplace_back(server_cell);
            }
        }

        return container;
    }

    return {};
}

celeritas::server_cell_repository::void_awaitable_type celeritas::server_cell_repository::load_from_db()
{
    try
    {
        co_return co_await do_load_from_db();
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(auth_channel, error) << "load server cell from db error: " << error.what();
    }
    catch (...)
    {
        LOG_CHANNEL(auth_channel, fatal) << "load server cell from db unknown error.";
    }
}

celeritas::server_cell_repository::void_awaitable_type celeritas::server_cell_repository::do_load_from_db()
{
    const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_auth_db_name.data());

    const auto apps_result = co_await mysql_pool->select_all(server_cell::get_select(database_type::mysql), server_cell::get_database_field_container());

    server_cell_type server_cell_type{};
    game_server_type game_server_type{};
    app_id_server_type app_id_server_type{};
    for (const auto& row : apps_result)
    {
        const server_cell server_cell{ row };
        server_cell_type.emplace(server_cell.get_cell_id(), server_cell);
        game_server_type.emplace(server_cell.get_game_server_id(), server_cell);

        const auto app_id = server_cell.get_app_id();
        app_id_server_type[app_id].emplace_back(server_cell);
    }

    for (auto& element : app_id_server_type | std::views::values)
    {
        std::ranges::sort(element, [](const auto& lhs, const auto& rhs) {
            return lhs.get_launch_time() < rhs.get_launch_time();
        });
    }

    std::unique_lock lock{ mutex_ };
    server_cell_ = std::move(server_cell_type);
    game_server_ = std::move(game_server_type);
    app_id_server_ = std::move(app_id_server_type);
}

celeritas::server_cell_repository::void_awaitable_type celeritas::server_cell_repository::load_from_db(const int64_t cell_id)
{
    try
    {
        co_return co_await do_load_from_db(cell_id);
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(auth_channel, error) << "load server cell from db error: " << error.what();
    }
    catch (...)
    {
        LOG_CHANNEL(auth_channel, fatal) << "load server cell from db unknown error.";
    }
}

celeritas::server_cell_repository::void_awaitable_type celeritas::server_cell_repository::do_load_from_db(const int64_t cell_id)
{
    const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_auth_db_name.data());

    if (const auto optional_server_cell = co_await mysql_pool->select_one(server_cell::get_select(database_type::mysql, cell_id), server_cell::get_database_field_container()))
    {
        const server_cell server_cell{ *optional_server_cell };

        std::unique_lock lock{ mutex_ };

        if (const auto iter = server_cell_.find(cell_id);
            iter != server_cell_.cend())
        {
            const auto app_id = iter->second.get_app_id();

            game_server_.erase(iter->second.get_game_server_id());
            server_cell_.erase(iter);

            if (const auto current = app_id_server_.find(app_id);
                current != app_id_server_.cend())
            {
                std::erase_if(current->second, [cell_id](const auto& element) {
                    return element.get_cell_id() == cell_id;
                });
            }
        }

        server_cell_.emplace(server_cell.get_cell_id(), server_cell);
        game_server_.emplace(server_cell.get_game_server_id(), server_cell);

        if (const auto current = app_id_server_.find(server_cell.get_app_id());
            current != app_id_server_.cend())
        {
            current->second.emplace_back(server_cell);
            std::ranges::sort(current->second, [](const auto& lhs, const auto& rhs) {
                return lhs.get_launch_time() < rhs.get_launch_time();
            });
        }
    }
}
