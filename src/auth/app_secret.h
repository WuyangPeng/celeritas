#pragma once

#include "database/generated/mysql/auth/apps.h"

#include <boost/asio.hpp>

#include <shared_mutex>
#include <unordered_map>

namespace celeritas
{
    class app_secret
    {
    public:
        using class_type = app_secret;
        using io_context_type = boost::asio::io_context;
        using void_awaitable_type = boost::asio::awaitable<void>;

        [[nodiscard]] static app_secret& get_instance();

        [[nodiscard]] std::string get_key(int app_id);

        void reload_from_db(io_context_type& io_context, int app_id);

        void load_from_db(io_context_type& io_context);

    private:
        using apps_type = std::unordered_map<int, apps>;

        app_secret() noexcept = default;

        [[nodiscard]] void_awaitable_type load_from_db();

        [[nodiscard]] void_awaitable_type do_load_from_db();

        [[nodiscard]] void_awaitable_type load_from_db(int app_id);

        [[nodiscard]] void_awaitable_type do_load_from_db(int app_id);

        apps_type apps_;
        std::shared_mutex mutex_;
    };
}