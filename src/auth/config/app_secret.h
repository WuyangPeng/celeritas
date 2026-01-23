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
        using any_io_executor = boost::asio::any_io_executor;

        [[nodiscard]] static app_secret& get_instance();

        [[nodiscard]] std::string get_key(int64_t app_id);

        [[nodiscard]] apps get_apps(int64_t app_id);

        void reload_from_db(const any_io_executor& any_io_executor, int64_t app_id);

        void load_from_db(const any_io_executor& any_io_executor);

    private:
        using apps_type = std::unordered_map<int64_t, apps>;

        app_secret() noexcept = default;

        [[nodiscard]] void_awaitable_type load_from_db();

        [[nodiscard]] void_awaitable_type load_from_db(int64_t app_id);

        apps_type apps_;
        std::shared_mutex mutex_;
    };
}