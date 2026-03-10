#pragma once

#include "database/generated/mysql/auth/sms_providers.h"

#include <boost/asio.hpp>

#include <shared_mutex>
#include <unordered_map>

namespace celeritas
{
    class app_sms_providers
    {
    public:
        using class_type = app_sms_providers;
        using any_io_executor = boost::asio::any_io_executor;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using const_sms_providers_shared_ptr = std::shared_ptr<const sms_providers>;

        [[nodiscard]] static app_sms_providers& get_instance();

        [[nodiscard]] const_sms_providers_shared_ptr get_sms_providers(int64_t provider_id);

        static void reload_from_db(const any_io_executor& any_io_executor, int64_t provider_id);

        static void load_from_db(const any_io_executor& any_io_executor);

    private:
        using optional_database_entity_change = std::optional<database_entity_change>;
        using database_entity_change_container = std::vector<database_entity_change>;
        using sms_providers_container = std::unordered_map<int64_t, const_sms_providers_shared_ptr>;

        app_sms_providers() noexcept = default;

        [[nodiscard]] void_awaitable_type load_from_db();

        [[nodiscard]] void_awaitable_type load_from_db(int64_t provider_id);

        void add_sms_provider(const optional_database_entity_change& optional_provider);

        [[nodiscard]] static sms_providers_container get_sms_providers_container(const database_entity_change_container& apps_result);

        sms_providers_container sms_providers_;
        std::shared_mutex mutex_;
    };
}