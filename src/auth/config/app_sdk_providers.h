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
        using io_context_type = boost::asio::io_context;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using any_io_executor = boost::asio::any_io_executor;

        [[nodiscard]] static app_sdk_providers& get_instance();

        [[nodiscard]] sdk_providers get_sdk_providers(const sdk_providers_key& sdk_providers_key);

        void reload_from_db(const any_io_executor& any_io_executor, int64_t sdk_id);

        void load_from_db(const any_io_executor& any_io_executor);

    private:
        using sdk_providers_type = std::unordered_map<sdk_providers_key, sdk_providers>;

        app_sdk_providers() noexcept = default;

        [[nodiscard]] void_awaitable_type load_from_db();

        [[nodiscard]] void_awaitable_type load_from_db(int64_t sdk_id);

        sdk_providers_type sdk_providers_;
        std::shared_mutex mutex_;
    };
}