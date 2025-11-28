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
        using io_context_type = boost::asio::io_context;
        using void_awaitable_type = boost::asio::awaitable<void>;

        [[nodiscard]] static app_sms_providers& get_instance();

        [[nodiscard]] sms_providers get_sms_providers(int64_t provider_id);

        void reload_from_db(io_context_type& io_context, int64_t provider_id);

        void load_from_db(io_context_type& io_context);

    private:
        using sms_providers_type = std::unordered_map<int64_t, sms_providers>;

        app_sms_providers() noexcept = default;

        [[nodiscard]] void_awaitable_type load_from_db();

        [[nodiscard]] void_awaitable_type do_load_from_db();

        [[nodiscard]] void_awaitable_type load_from_db(int64_t provider_id);

        [[nodiscard]] void_awaitable_type do_load_from_db(int64_t provider_id);

        sms_providers_type sms_providers_;
        std::shared_mutex mutex_;
    };
}