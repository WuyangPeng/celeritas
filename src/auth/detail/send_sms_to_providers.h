#pragma once

#include "database/generated/mysql/auth/sms_providers.h"
#include "database/generated/redis/auth/sms_code.h"

#include <boost/asio/awaitable.hpp>

namespace celeritas
{
    class send_sms_to_providers
    {
    public:
        using class_type = send_sms_to_providers;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using send_sms_to_providers_unique_ptr = std::unique_ptr<class_type>;

        send_sms_to_providers(const sms_code& sms_code, const sms_providers& sms_providers);

        virtual ~send_sms_to_providers() noexcept = default;

        send_sms_to_providers(const send_sms_to_providers& rhs) noexcept = default;

        send_sms_to_providers& operator=(const send_sms_to_providers& rhs) noexcept = default;

        send_sms_to_providers(send_sms_to_providers&& rhs) noexcept = default;

        send_sms_to_providers& operator=(send_sms_to_providers&& rhs) noexcept = default;

        [[nodiscard]] virtual void_awaitable_type execute() = 0;

        [[nodiscard]] static send_sms_to_providers_unique_ptr create(const sms_code& sms_code, const sms_providers& sms_providers);

    private:
        sms_code sms_code_;
        sms_providers sms_providers_;
    };
}