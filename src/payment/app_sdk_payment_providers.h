#pragma once

#include "sdk_payment_providers_key.h"
#include "database/generated/mysql/payment/sdk_payment_providers.h"

#include <boost/asio.hpp>

#include <shared_mutex>
#include <unordered_map>

namespace celeritas
{
    class app_sdk_payment_providers
    {
    public:
        using class_type = app_sdk_payment_providers;
        using io_context_type = boost::asio::io_context;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using sdk_payment_providers_type = std::unordered_map<sdk_payment_providers_key, sdk_payment_providers>;

        [[nodiscard]] static app_sdk_payment_providers& get_instance();

        [[nodiscard]] sdk_payment_providers get_sdk_payment_providers(const sdk_payment_providers_key& sdk_payment_providers_key);

        [[nodiscard]] sdk_payment_providers get_sdk_payment_providers(int64_t sdk_id);

        void reload_from_db(io_context_type& io_context, int64_t sdk_id);

        void load_from_db(io_context_type& io_context);

        [[nodiscard]] sdk_payment_providers_type get_sdk_payment_providers();

    private:
        app_sdk_payment_providers() noexcept = default;

        [[nodiscard]] void_awaitable_type load_from_db();

        [[nodiscard]] void_awaitable_type do_load_from_db();

        [[nodiscard]] void_awaitable_type load_from_db(int64_t sdk_id);

        [[nodiscard]] void_awaitable_type do_load_from_db(int64_t sdk_id);

        sdk_payment_providers_type sdk_payment_providers_;
        std::shared_mutex mutex_;
    };
}