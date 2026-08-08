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
        using optional_string = std::optional<std::string>;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using optional_server_cell = std::optional<server_cell>;
        using server_cell_container = std::vector<server_cell>;

        [[nodiscard]] static server_cell_repository& get_instance();

        static void reload_from_db(const any_io_executor& any_io_executor, int64_t cell_id);

        static void load_from_db(const any_io_executor& any_io_executor);

        [[nodiscard]] optional_server_cell get_server_cell(const std::string& game_server_id);

        [[nodiscard]] optional_server_cell get_last_server_cell(int64_t app_id);

        [[nodiscard]] server_cell_container get_server_cell_by_app_id(int64_t app_id, const optional_string& zone);

        [[nodiscard]] std::vector<std::string> get_all_zones(int64_t app_id);

    private:
        using server_cell_mapping = std::unordered_map<int64_t, server_cell>;
        using game_server_container = std::unordered_map<std::string, server_cell>;
        using app_id_server_container = std::unordered_map<int64_t, server_cell_container>;

        server_cell_repository() noexcept = default;

        [[nodiscard]] void_awaitable_type load_from_db();

        [[nodiscard]] void_awaitable_type load_from_db(int64_t cell_id);

        server_cell_mapping server_cell_;
        game_server_container game_server_;
        app_id_server_container app_id_server_;
        std::shared_mutex mutex_;
    };
}