#pragma once

#include "database/generated/mysql/auth/email_providers.h"

#include <boost/asio.hpp>

#include <shared_mutex>
#include <unordered_map>

namespace celeritas
{
    class app_email_providers
    {
    public:
        using class_type = app_email_providers;
        using any_io_executor = boost::asio::any_io_executor;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using const_email_providers_shared_ptr = std::shared_ptr<const email_providers>;

        [[nodiscard]] static app_email_providers& get_instance();

        [[nodiscard]] const_email_providers_shared_ptr get_email_providers(int64_t provider_id);

        void reload_from_db(const any_io_executor& any_io_executor, int64_t provider_id);

        void load_from_db(const any_io_executor& any_io_executor);

    private:
        using email_providers_container = std::unordered_map<int64_t, const_email_providers_shared_ptr>;

        app_email_providers() noexcept = default;

        [[nodiscard]] void_awaitable_type load_from_db();

        [[nodiscard]] void_awaitable_type load_from_db(int64_t provider_id);

        email_providers_container email_providers_;
        std::shared_mutex mutex_;
    };
}