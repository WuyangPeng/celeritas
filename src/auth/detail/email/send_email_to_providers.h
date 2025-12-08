#pragma once

#include "database/generated/mysql/auth/email_providers.h"
#include "database/generated/redis/auth/email_code.h"

#include <boost/asio/awaitable.hpp>

namespace celeritas
{
    class send_email_to_providers
    {
    public:
        using class_type = send_email_to_providers;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using send_email_to_providers_unique_ptr = std::unique_ptr<class_type>;

        send_email_to_providers(email_code email_code, email_providers email_providers);

        virtual ~send_email_to_providers() noexcept = default;

        send_email_to_providers(const send_email_to_providers& rhs) noexcept = default;

        send_email_to_providers& operator=(const send_email_to_providers& rhs) noexcept = default;

        send_email_to_providers(send_email_to_providers&& rhs) noexcept = default;

        send_email_to_providers& operator=(send_email_to_providers&& rhs) noexcept = default;

        [[nodiscard]] virtual void_awaitable_type execute() = 0;

        [[nodiscard]] static send_email_to_providers_unique_ptr create(email_code email_code, email_providers email_providers);

    private:
        email_code email_code_;
        email_providers email_providers_;
    };
}