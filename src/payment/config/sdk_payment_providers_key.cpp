#include "sdk_payment_providers_key.h"

celeritas::sdk_payment_providers_key::sdk_payment_providers_key(const int64_t app_id, const payment_platform_type payment_platform_type)
    : app_id_{ app_id }, payment_platform_type_{ payment_platform_type }
{
}

int64_t celeritas::sdk_payment_providers_key::get_app_id() const
{
    return app_id_;
}

celeritas::payment_platform_type celeritas::sdk_payment_providers_key::get_payment_platform_type() const
{
    return payment_platform_type_;
}

bool celeritas::operator==(const sdk_payment_providers_key& lhs, const sdk_payment_providers_key& rhs)
{
    return lhs.get_app_id() == rhs.get_app_id() && lhs.get_payment_platform_type() == rhs.get_payment_platform_type();
}

std::size_t std::hash<celeritas::sdk_payment_providers_key>::operator()(const celeritas::sdk_payment_providers_key& key) const noexcept
{
    size_t seed{};

    seed ^= std::hash<int64_t>()(key.get_app_id()) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= std::hash<int>()(static_cast<int>(key.get_payment_platform_type())) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

    return seed;
}