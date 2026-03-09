#pragma once

#include "sdk_providers_key.h"
#include "database/generated/mysql/auth/sdk_providers.h"

#include <boost/asio.hpp>

#include <shared_mutex>
#include <unordered_map>

namespace celeritas
{
    class app_sdk_providers
    {
    public:
        using class_type = app_sdk_providers;
        using any_io_executor = boost::asio::any_io_executor;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using const_sdk_providers_shared_ptr = std::shared_ptr<const sdk_providers>;

        [[nodiscard]] static app_sdk_providers& get_instance();

        [[nodiscard]] const_sdk_providers_shared_ptr get_sdk_providers(const sdk_providers_key& sdk_providers_key);

        static void reload_from_db(const any_io_executor& any_io_executor, int64_t sdk_id);

        static void load_from_db(const any_io_executor& any_io_executor);

    private:
        using optional_database_entity_change = std::optional<database_entity_change>;
        using database_entity_change_container = std::vector<database_entity_change>;
        using sdk_providers_container = std::unordered_map<sdk_providers_key, const_sdk_providers_shared_ptr>;

        app_sdk_providers() noexcept = default;

        [[nodiscard]] void_awaitable_type load_from_db();

        [[nodiscard]] void_awaitable_type load_from_db(int64_t sdk_id);

        void add_sdk_provider(const optional_database_entity_change& optional_provider);

        [[nodiscard]] static sdk_providers_container get_sdk_providers_container(const database_entity_change_container& apps_result);

        sdk_providers_container sdk_providers_;
        std::shared_mutex mutex_;
    };
}