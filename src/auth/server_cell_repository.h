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
        using io_context_type = boost::asio::io_context;
        using void_awaitable_type = boost::asio::awaitable<void>;

        [[nodiscard]] static server_cell_repository& get_instance();

        void reload_from_db(io_context_type& io_context, int64_t cell_id);

        void load_from_db(io_context_type& io_context);

    private:
        using server_cell_type = std::unordered_map<int64_t, server_cell>;

        server_cell_repository() noexcept = default;

        [[nodiscard]] void_awaitable_type load_from_db();

        [[nodiscard]] void_awaitable_type do_load_from_db();

        [[nodiscard]] void_awaitable_type load_from_db(int64_t cell_id);

        [[nodiscard]] void_awaitable_type do_load_from_db(int64_t cell_id);

        server_cell_type server_cell_;
        std::shared_mutex mutex_;
    };
}