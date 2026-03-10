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
        using any_io_executor = boost::asio::any_io_executor;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using const_apps_shared_ptr = std::shared_ptr<const apps>;

        [[nodiscard]] static app_secret& get_instance();

        [[nodiscard]] std::string get_key(int64_t app_id);

        [[nodiscard]] const_apps_shared_ptr get_apps(int64_t app_id);

        static void reload_from_db(const any_io_executor& any_io_executor, int64_t app_id);

        static void load_from_db(const any_io_executor& any_io_executor);

    private:
        using optional_database_entity_change = std::optional<database_entity_change>;
        using database_entity_change_container = std::vector<database_entity_change>;
        using apps_container = std::unordered_map<int64_t, const_apps_shared_ptr>;

        app_secret() noexcept = default;

        [[nodiscard]] void_awaitable_type load_from_db();

        [[nodiscard]] void_awaitable_type load_from_db(int64_t app_id);

        void add_app_provider(const optional_database_entity_change& optional_provider);

        [[nodiscard]] static apps_container get_app_providers_container(const database_entity_change_container& apps_result);

        apps_container apps_;
        std::shared_mutex mutex_;
    };
}