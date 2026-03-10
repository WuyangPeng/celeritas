#pragma once

#include "database/generated/mysql/auth/server_cell.h"

#include <boost/asio.hpp>

#include <shared_mutex>
#include <unordered_map>

namespace celeritas
{
    class server_cell_repository
    {
    public:
        using class_type = server_cell_repository;
        using any_io_executor = boost::asio::any_io_executor;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using optional_server_cell_type = std::optional<server_cell>;
        using server_cell_container_type = std::vector<server_cell>;
        using optional_string = std::optional<std::string>;

        [[nodiscard]] static server_cell_repository& get_instance();

        void reload_from_db(const any_io_executor& any_io_executor, int64_t cell_id);

        void load_from_db(const any_io_executor& any_io_executor);

        [[nodiscard]] optional_server_cell_type get_server_cell(const std::string& game_server_id);

        [[nodiscard]] optional_server_cell_type get_last_server_cell(int64_t app_id);

        [[nodiscard]] server_cell_container_type get_server_cell_by_app_id(int64_t app_id, const optional_string& zone);

    private:
        using server_cell_type = std::unordered_map<int64_t, server_cell>;
        using game_server_type = std::unordered_map<std::string, server_cell>;
        using app_id_server_type = std::unordered_map<int64_t, server_cell_container_type>;

        server_cell_repository() noexcept = default;

        [[nodiscard]] void_awaitable_type load_from_db();

        [[nodiscard]] void_awaitable_type load_from_db(int64_t cell_id);

        server_cell_type server_cell_;
        game_server_type game_server_;
        app_id_server_type app_id_server_;
        std::shared_mutex mutex_;
    };
}