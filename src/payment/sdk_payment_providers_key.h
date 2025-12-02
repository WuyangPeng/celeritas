#pragma once

#include "payment_fwd.h"

#include <cstdint>
#include <functional>

namespace celeritas
{
    class sdk_payment_providers_key
    {
    public:
        using class_type = sdk_payment_providers_key;

        sdk_payment_providers_key(int64_t app_id, payment_platform_type payment_platform_type);

        [[nodiscard]] int64_t get_app_id() const;

        [[nodiscard]] payment_platform_type get_payment_platform_type() const;

    private:
        int64_t app_id_;
        payment_platform_type payment_platform_type_;
    };

    [[nodiscard]] bool operator==(const sdk_payment_providers_key& lhs, const sdk_payment_providers_key& rhs);
}

template <>
struct std::hash<celeritas::sdk_payment_providers_key>
{
    [[nodiscard]] size_t operator()(const celeritas::sdk_payment_providers_key& key) const noexcept;
};
